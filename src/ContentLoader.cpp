#include "ContentLoader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<Event> ContentLoader::LoadEvents(const std::string &filepath) {
  std::vector<Event> events;
  std::ifstream file(filepath);

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filepath << std::endl;
    return events;
  }

  std::string line;
  Event currentEvent;
  bool parsingEvent = false;

  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string command;
    ss >> command;

    // Helper to get rest of line
    auto GetRest = [&]() {
      std::string rest;
      std::getline(ss, rest);
      if (!rest.empty() && rest[0] == ' ')
        rest = rest.substr(1);
      return rest;
    };

    if (command == "EVENT") {
      if (parsingEvent) {
        events.push_back(currentEvent);
      }
      currentEvent = Event();
      currentEvent.id = GetRest();
      parsingEvent = true;
    } else if (command == "TITLE") {
      if (parsingEvent)
        currentEvent.title = GetRest();
    } else if (command == "DESC") {
      if (parsingEvent) {
        if (!currentEvent.description.empty())
          currentEvent.description += "\n";
        currentEvent.description += GetRest();
      }
    } else if (command == "REQ") {
      if (parsingEvent)
        currentEvent.reqs.push_back(GetRest());
    } else if (command == "TAG") {
      if (parsingEvent)
        currentEvent.tags.push_back(GetRest());
    } else if (command == "CHOICE") {
      if (parsingEvent) {
        Choice c;
        c.text = GetRest();
        currentEvent.choices.push_back(c);
      }
    } else if (command == "EFFECT") {
      if (parsingEvent && !currentEvent.choices.empty()) {
        Choice &lastChoice = currentEvent.choices.back();
        // Parse stats: "resolve 1 dread -1"
        std::string key;
        int val;
        while (ss >> key >> val) {
          if (key == "resolve")
            lastChoice.statChanges.resolve = val;
          else if (key == "dread")
            lastChoice.statChanges.dread = val;
          else if (key == "clarity")
            lastChoice.statChanges.clarity = val;
          else if (key == "pride")
            lastChoice.statChanges.pride = val;
          else if (key == "compassion")
            lastChoice.statChanges.compassion = val;
        }
      }
    } else if (command == "REWARD") {
      if (parsingEvent && !currentEvent.choices.empty()) {
        std::string id = GetRest();
        currentEvent.choices.back().shadowRewardId = id;
      }
    } else if (command == "ADDFLAG") {
      if (parsingEvent && !currentEvent.choices.empty()) {
        currentEvent.choices.back().addFlags.push_back(GetRest());
      }
    } else if (command == "CHALLENGE") {
      if (parsingEvent && !currentEvent.choices.empty()) {
        Choice &c = currentEvent.choices.back();
        c.hasChallenge = true;
        ss >> c.challenge.statName >> c.challenge.difficulty; // e.g. "dread 12"
      }
    } else if (command == "WinEFFECT") {
      if (parsingEvent && !currentEvent.choices.empty() &&
          currentEvent.choices.back().hasChallenge) {
        // Parse stats for success
        std::string key;
        int val;
        while (ss >> key >> val) {
          Stats &s = currentEvent.choices.back().challenge.successEffect;
          if (key == "resolve")
            s.resolve = val;
          else if (key == "dread")
            s.dread = val;
          else if (key == "clarity")
            s.clarity = val;
          else if (key == "pride")
            s.pride = val;
          else if (key == "compassion")
            s.compassion = val;
        }
      }
    } else if (command == "FailEFFECT") {
      if (parsingEvent && !currentEvent.choices.empty() &&
          currentEvent.choices.back().hasChallenge) {
        std::string key;
        int val;
        while (ss >> key >> val) {
          Stats &s = currentEvent.choices.back().challenge.failureEffect;
          if (key == "resolve")
            s.resolve = val;
          else if (key == "dread")
            s.dread = val;
          else if (key == "clarity")
            s.clarity = val;
          else if (key == "pride")
            s.pride = val;
          else if (key == "compassion")
            s.compassion = val;
        }
      }
    } else if (command == "RewWin") {
      if (parsingEvent && !currentEvent.choices.empty()) {
        currentEvent.choices.back().challenge.winRewardId = GetRest();
      }
    } else if (command == "RewFail") {
      if (parsingEvent && !currentEvent.choices.empty()) {
        currentEvent.choices.back().challenge.failRewardId = GetRest();
      }
    } else if (command == "END") {
      if (parsingEvent) {
        events.push_back(currentEvent);
        parsingEvent = false;
      }
    }
  }

  if (parsingEvent) {
    events.push_back(currentEvent);
  }

  return events;
}
