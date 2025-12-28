#include "Game.hpp"
#include "Input.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

Game::Game() : m_running(true) {
  m_eventLib.InitSampleEvents(); // Now loads from file
  m_shadowLib.InitSampleShadows();
  // Ensure data directory exists
  std::filesystem::create_directories("data");
}

void Game::Run() {
  while (m_running) {
    if (m_state.GetPhase() == GamePhase::MENU) {
      MainMenu();
    } else if (m_state.GetPhase() == GamePhase::PLAYING) {
      GameLoop();
    } else if (m_state.GetPhase() == GamePhase::GAMEOVER) {
      m_renderer.RenderGameOver(GetViewModel());
      Input::GetInt();
      m_state.SetPhase(GamePhase::MENU);
    } else if (m_state.GetPhase() == GamePhase::WON) {
      m_renderer.RenderWin(GetViewModel());
      Input::GetInt();
      m_state.SetPhase(GamePhase::MENU);
    }
  }
}

void Game::MainMenu() {
  m_renderer.RenderMenu();
  int choice = Input::GetInt();

  switch (choice) {
  case 1: { // New Run
    std::cout << "Enter Seed (integer): ";
    int seed = Input::GetInt();
    if (seed == -1)
      seed = 12345;
    m_state.Init(seed);
    m_log.clear();
    AddLog("Welcome to the Shadow Index.");
    PickNextEvent(); // Load first room
    break;
  }
  case 2: // Load Run
    if (LoadGame()) {
      m_log.clear();
      AddLog("Game Loaded.");
      // GameLoop() is called by Run() if phase is PLAYING
    } else {
      std::cout << "Failed to load save file (or none exists).\n";
      std::cout << "Press Enter to continue.\n";
      Input::GetInt();
    }
    break;
  case 3: // Quit
    m_running = false;
    break;
  default:
    break;
  }
}

void Game::GameLoop() {
  ViewModel vm = GetViewModel();
  m_renderer.RenderGame(vm);

  int choice = Input::GetInt();
  HandleChoice(choice);

  // Check Game Over
  if (m_state.GetBaseStats().resolve <= 0) {
    m_state.SetPhase(GamePhase::GAMEOVER);
  } else if (m_state.GetDepth() >= 10) {
    m_state.SetPhase(GamePhase::WON);
  }
}

void Game::AddLog(const std::string &msg) {
  m_log.push_back(msg);
  if (m_log.size() > 50)
    m_log.erase(m_log.begin());
}

void Game::PickNextEvent() {
  // Deterministic selection
  const Event *e = m_eventLib.GetRandomEvent(m_state.GetRng(), m_state);
  if (e) {
    m_state.SetCurrentEventId(e->id);

    // Trigger: ON_ENTER_ROOM
    // Iterate all active shadows
    for (const auto &sid : m_state.GetInventory()) {
      const Shadow *s = m_shadowLib.GetShadow(sid);
      if (!s)
        continue;
      for (const auto &t : s->triggers) {
        if (t.type == TriggerType::ON_ENTER_ROOM) {
          m_state.UpdateStats(t.statChange);
          // Could queue a message here "Whispering Stone grants +1 Clarity"
        }
      }
    }
  } else {
    m_state.SetCurrentEventId("error");
  }
}

void Game::HandleChoice(int choice) {
  const Event *evt = m_eventLib.GetEvent(m_state.GetCurrentEventId());
  if (!evt)
    return;

  // Adjust for 1-based input
  int idx = choice - 1;
  if (idx < 0 || idx >= static_cast<int>(evt->choices.size())) {
    return;
  }

  const auto &c = evt->choices[idx];

  if (c.hasChallenge) {
    int roll = m_state.GetRng().Int(1, 20);
    int statBonus = 0;
    Stats currentStats = m_state.GetTotalStats(m_shadowLib);

    // Simple map from string to stat value (since we don't have
    // GetStat(string))
    std::string s = c.challenge.statName;
    if (s == "resolve")
      statBonus = currentStats.resolve;
    else if (s == "dread")
      statBonus = currentStats.dread;
    else if (s == "clarity")
      statBonus = currentStats.clarity;
    else if (s == "pride")
      statBonus = currentStats.pride;
    else if (s == "compassion")
      statBonus = currentStats.compassion;

    int total = roll + statBonus;
    bool success = (total >= c.challenge.difficulty);

    std::string resultMsg = "Rolled " + std::to_string(roll) + " + " +
                            std::to_string(statBonus) + " (" + s +
                            ") = " + std::to_string(total) + " vs DC " +
                            std::to_string(c.challenge.difficulty);

    if (success) {
      AddLog(resultMsg + ". SUCCESS!");
      m_state.UpdateStats(c.challenge.successEffect);
      if (!c.challenge.winRewardId.empty())
        m_state.AddShadow(c.challenge.winRewardId);
    } else {
      AddLog(resultMsg + ". FAILURE!");
      m_state.UpdateStats(c.challenge.failureEffect);
      if (!c.challenge.failRewardId.empty())
        m_state.AddShadow(c.challenge.failRewardId);
    }

  } else {
    // Standard (No challenge)
    m_state.UpdateStats(c.statChanges);
    if (!c.shadowRewardId.empty()) {
      m_state.AddShadow(c.shadowRewardId);
      AddLog("Acquired " + c.shadowRewardId);
    }
  }

  // Add flags for the choice
  for (const auto &flag : c.addFlags) {
    m_state.AddFlag(flag);
    // Optional: Log flag addition for debug? "Meta-changed: " + flag
  }

  m_state.AdvanceDepth();
  PickNextEvent();
  SaveGame();
}

ViewModel Game::GetViewModel() {
  ViewModel vm;
  vm.seed = m_state.GetSeed();
  vm.depth = m_state.GetDepth();
  vm.stats = m_state.GetTotalStats(m_shadowLib); // Use total stats!

  // Populate active shadows for UI
  for (const auto &sid : m_state.GetInventory()) {
    const Shadow *s = m_shadowLib.GetShadow(sid);
    if (s)
      vm.activeShadows.push_back(s->name);
    else
      vm.activeShadows.push_back(sid);
  }

  vm.log = m_log;

  const Event *evt = m_eventLib.GetEvent(m_state.GetCurrentEventId());
  if (evt) {
    vm.roomTitle = evt->title;
    vm.roomLines = {evt->description};

    vm.options.clear();
    for (const auto &c : evt->choices) {
      std::string text = c.text;
      if (c.hasChallenge) {
        text += " [" + c.challenge.statName + " DC " +
                std::to_string(c.challenge.difficulty) + "]";
      }
      if (!c.shadowRewardId.empty()) {
        const Shadow *s = m_shadowLib.GetShadow(c.shadowRewardId);
        if (s)
          text += " [REWARD: " + s->name + "]";
      }
      vm.options.push_back(text);
    }
  } else {
    vm.roomTitle = "Unknown Event";
    vm.roomLines = {"You are lost in the void. (Event ID not found)"};
    vm.options = {"Continue"};
  }

  return vm;
}

void Game::SaveGame() {
  std::ofstream ofs("data/savegame.txt");
  if (ofs.is_open()) {
    m_state.Serialize(ofs);
    ofs.close();
  }
}

bool Game::LoadGame() {
  std::ifstream ifs("data/savegame.txt");
  if (ifs.is_open()) {
    bool success = m_state.Deserialize(ifs);
    ifs.close();
    if (success) {
      m_state.SetPhase(GamePhase::PLAYING);
      return true;
    }
  }
  return false;
}
