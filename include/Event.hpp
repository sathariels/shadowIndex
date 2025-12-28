#ifndef EVENT_HPP
#define EVENT_HPP

#include "Stats.hpp"
#include <string>
#include <vector>

struct Challenge {
  std::string statName; // "dread", "clarity", etc.
  int difficulty;
  Stats successEffect;
  Stats failureEffect;
  std::string winRewardId;
  std::string failRewardId;
};

struct Choice {
  std::string text;
  Stats statChanges; // Immediate effect
  std::string shadowRewardId;
  std::vector<std::string> addFlags; // Flags to set if this choice is taken

  bool hasChallenge = false;
  Challenge challenge;
};

struct Event {
  std::string id;
  std::string title;
  std::string description;
  std::vector<Choice> choices;

  std::vector<std::string> reqs; // Flags required to see this event
  std::vector<std::string> tags; // Optional categorization
};

#endif // EVENT_HPP
