#include "../include/EventLibrary.hpp"
#include "ContentLoader.hpp"
#include "GameState.hpp"
#include <iostream>

void EventLibrary::InitSampleEvents() {
  m_events.clear();
  m_idToIndex.clear();

  m_events = ContentLoader::LoadEvents("data/events/core_events.txt");

  for (size_t i = 0; i < m_events.size(); ++i) {
    m_idToIndex[m_events[i].id] = i;
  }

  std::cout << "Loaded " << m_events.size() << " events." << std::endl;
}

const Event *EventLibrary::GetEvent(const std::string &id) const {
  auto it = m_idToIndex.find(id);
  if (it != m_idToIndex.end()) {
    return &m_events[it->second];
  }
  return nullptr;
}

const Event *EventLibrary::GetRandomEvent(Rng &rng,
                                          const GameState &state) const {
  std::vector<const Event *> candidates;
  for (const auto &event : m_events) {
    bool valid = true;
    for (const auto &req : event.reqs) {
      if (!state.HasFlag(req)) {
        valid = false;
        break;
      }
    }
    if (valid) {
      candidates.push_back(&event);
    }
  }

  if (candidates.empty())
    return nullptr;
  int idx = rng.Int(0, candidates.size() - 1);
  return candidates[idx];
}
