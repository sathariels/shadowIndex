#ifndef VIEWMODEL_HPP
#define VIEWMODEL_HPP

#include "Stats.hpp"
#include <string>
#include <vector>

struct ViewModel {
  // Run Info
  int seed;
  int depth;

  // Player Status
  Stats stats;
  std::vector<std::string> activeShadows;
  std::vector<std::string> log; // History of actions/rolls
  bool gameOver = false;

  // Main Content
  std::string roomTitle;
  std::vector<std::string>
      roomLines; // Pre-wrapped or raw text? Let's say raw paragraphs for now,
                 // Renderer handles wrapping or easy splitting.

  // Choices
  std::vector<std::string> options;
};

#endif // VIEWMODEL_HPP
