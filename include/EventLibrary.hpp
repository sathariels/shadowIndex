#ifndef EVENTLIBRARY_HPP
#define EVENTLIBRARY_HPP

#include "Event.hpp"
#include "Rng.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class GameState;

class EventLibrary {
public:
  void InitSampleEvents();

  // Returns a pointer to the event or nullptr if not found
  const Event *GetEvent(const std::string &id) const;

  // Returns a random event from the library
  const Event *GetRandomEvent(Rng &rng, const GameState &state) const;

private:
  std::vector<Event> m_events;
  std::unordered_map<std::string, size_t> m_idToIndex;
};

#endif // EVENTLIBRARY_HPP
