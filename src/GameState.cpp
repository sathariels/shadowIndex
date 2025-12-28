#include "GameState.hpp"
#include "ShadowLibrary.hpp"
#include <iostream>

GameState::GameState()
    : m_phase(GamePhase::MENU), m_seed(0), m_depth(0), m_stats{0, 0, 0, 0, 0} {}

void GameState::Init(int seed) {
  m_seed = seed;
  m_depth = 1;
  m_stats = {10, 0, 5, 5, 5}; // Base stats
  m_phase = GamePhase::PLAYING;
  m_rng.Seed(seed);
  m_currentEventId = "";
  m_inventory.clear();
  m_storyFlags.clear();
}

void GameState::AdvanceDepth() { m_depth++; }

void GameState::SetCurrentEventId(const std::string &id) {
  m_currentEventId = id;
}

void GameState::Serialize(std::ostream &os) const {
  os << m_seed << "\n";
  os << m_depth << "\n";
  // Stats
  os << m_stats.resolve << " " << m_stats.dread << " " << m_stats.clarity << " "
     << m_stats.pride << " " << m_stats.compassion << "\n";

  os << m_currentEventId << "\n";

  // Inventory
  os << m_inventory.size() << "\n";
  for (const auto &item : m_inventory) {
    os << item << "\n";
  }

  // Story Flags
  os << m_storyFlags.size() << "\n";
  for (const auto &flag : m_storyFlags) {
    os << flag << "\n";
  }
}

bool GameState::Deserialize(std::istream &is) {
  if (!(is >> m_seed))
    return false;
  if (!(is >> m_depth))
    return false;

  if (!(is >> m_stats.resolve >> m_stats.dread >> m_stats.clarity >>
        m_stats.pride >> m_stats.compassion))
    return false;

  is >> std::ws; // consume whitespace before string getline or token
  if (!(is >> m_currentEventId))
    return false; // Assuming event ID is single token

  size_t invSize;
  if (!(is >> invSize))
    return false;

  m_inventory.clear();
  for (size_t i = 0; i < invSize; ++i) {
    std::string item;
    if (!(is >> item))
      return false;
    m_inventory.push_back(item);
  }

  size_t flagSize;
  if (is >> flagSize) {
    m_storyFlags.clear();
    for (size_t i = 0; i < flagSize; ++i) {
      std::string flag;
      if (is >> flag) {
        m_storyFlags.insert(flag);
      }
    }
  }

  // Key: Re-seed RNG to resume procedural generation logic if needed
  // But we are storing current state.
  // If we need to resume *stream* of events, we might need to fast forward RNG,
  // or just rely on 'currentEventId' being saved.
  // Ideally, we'd save RNG state too, OR just rely on seed + count.
  // For now, let's re-seed with 'm_seed'.
  // WARN: This resets RNG sequence. If player was deep in run,
  // future events might duplicate early ones if we don't advance RNG.
  // Simple fix: Save RNG state string if possible, or just accept re-seed.
  m_rng.Seed(m_seed);

  return true;
}

void GameState::UpdateStats(const Stats &delta) {
  m_stats += delta;
  if (m_stats.resolve < 0)
    m_stats.resolve = 0;
  if (m_stats.clarity < 0)
    m_stats.clarity = 0;
  if (m_stats.pride < 0)
    m_stats.pride = 0;
  if (m_stats.compassion < 0)
    m_stats.compassion = 0;
}

void GameState::AddShadow(const std::string &id) { m_inventory.push_back(id); }

Stats GameState::GetTotalStats(const ShadowLibrary &lib) const {
  Stats total = m_stats;
  for (const auto &shadowId : m_inventory) {
    const Shadow *s = lib.GetShadow(shadowId);
    if (s) {
      total += s->passiveStats;
    }
  }
  // Clamping again just in case passives reduce below zero?
  // Usually passives shouldn't break the game logic if negative, but UI might
  // look weird.
  return total;
}

void GameState::AddFlag(const std::string &flag) { m_storyFlags.insert(flag); }

bool GameState::HasFlag(const std::string &flag) const {
  return m_storyFlags.find(flag) != m_storyFlags.end();
}
