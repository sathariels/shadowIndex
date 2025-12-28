#ifndef CONTENTLOADER_HPP
#define CONTENTLOADER_HPP

#include "Event.hpp"
#include <string>
#include <vector>

class ContentLoader {
public:
  static std::vector<Event> LoadEvents(const std::string &filepath);
};

#endif // CONTENTLOADER_HPP
