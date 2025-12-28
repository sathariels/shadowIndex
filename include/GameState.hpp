#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Rng.hpp"
#include "Stats.hpp"
#include <string>
#include <unordered_set>
#include <vector>

// Forward declaration
class ShadowLibrary;

enum class GamePhase { MENU, PLAYING, GAMEOVER, WON };

class GameState {
public:
  GameState();

  void Init(int seed);

  // Serialization
  void Serialize(std::ostream &os) const;
  bool Deserialize(std::istream &is);

  // Accessors
  GamePhase GetPhase() const { return m_phase; }
  void SetPhase(GamePhase phase) { m_phase = phase; }

  int GetSeed() const { return m_seed; }
  int GetDepth() const { return m_depth; }
  // Now returns base stats. Use GetTotalStats for display/checks.
  const Stats &GetBaseStats() const { return m_stats; }
  const std::string &GetCurrentEventId() const { return m_currentEventId; }
  const std::vector<std::string> &GetInventory() const { return m_inventory; }

  Stats GetTotalStats(const ShadowLibrary &lib) const;

  // Services (Mutable)
  Rng &GetRng() { return m_rng; }

  // Modifiers
  void AdvanceDepth();
  void SetCurrentEventId(const std::string &id);
  void UpdateStats(const Stats &delta);
  void AddShadow(const std::string &id);

  void AddFlag(const std::string &flag);
  bool HasFlag(const std::string &flag) const;

private:
  GamePhase m_phase;
  int m_seed;
  int m_depth;
  Stats m_stats; // Base stats
  std::string m_currentEventId;
  std::vector<std::string> m_inventory;
  std::unordered_set<std::string> m_storyFlags;
  Rng m_rng;
};

#endif // GAMESTATE_HPP
