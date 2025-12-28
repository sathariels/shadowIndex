#ifndef GAME_HPP
#define GAME_HPP

#include "EventLibrary.hpp"
#include "GameState.hpp"
#include "Renderer.hpp"
#include "ShadowLibrary.hpp"
#include <memory>

class Game {
public:
  Game();
  void Run();

private:
  void MainMenu();
  void GameLoop();
  void HandleChoice(int choice);
  void AddLog(const std::string &msg);
  ViewModel GetViewModel();
  void PickNextEvent();
  void SaveGame();
  bool LoadGame();

  GameState m_state;
  Renderer m_renderer;
  EventLibrary m_eventLib;
  ShadowLibrary m_shadowLib;
  bool m_running;
  std::vector<std::string> m_log;
};

#endif // GAME_HPP
