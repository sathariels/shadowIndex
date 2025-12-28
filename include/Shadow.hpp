#ifndef SHADOW_HPP
#define SHADOW_HPP

#include "Stats.hpp"
#include <string>
#include <vector>

enum class TriggerType {
  ON_ENTER_ROOM,
  ON_STAT_CHECK // e.g., "If Dread > 10" (Future)
};

struct Trigger {
  TriggerType type;
  Stats statChange; // Effect when triggered
  std::string conditionParam;
};

struct Shadow {
  std::string id;
  std::string name;
  std::string description;
  Stats passiveStats; // Always applied
  std::vector<Trigger> triggers;
};

#endif // SHADOW_HPP
