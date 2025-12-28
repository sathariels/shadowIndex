#include "Renderer.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

// Basic ANSI codes could be used here, but keeping it simple with ASCII first.
// If user wants raw ASCII, we stick to standard chars using box drawing chars
// or just +-|

void Renderer::ClearScreen() {
  // Standard ANSI clear screen
  std::cout << "\033[2J\033[1;1H";
}

void Renderer::DrawDivider(int width) {
  std::cout << "+";
  for (int i = 0; i < width - 2; ++i)
    std::cout << "-";
  std::cout << "+" << std::endl;
}

// Helper to print a line respecting explicit newlines in string
void Renderer::PrintWrapped(const std::string &text, int width,
                            const std::string &prefix) {
  int maxLineLen = width - 4; // padding
  std::stringstream ss(text);
  std::string segment;

  // Split by newline first
  while (std::getline(ss, segment, '\n')) {
    std::istringstream words(segment);
    std::string word;
    std::string line = prefix;

    while (words >> word) {
      if (line.length() + word.length() + 1 > maxLineLen + prefix.length()) {
        std::cout << "| " << std::left << std::setw(width - 4) << line << " |"
                  << std::endl;
        line = prefix + word;
      } else {
        if (line.length() > prefix.length())
          line += " ";
        line += word;
      }
    }
    std::cout << "| " << std::left << std::setw(width - 4) << line << " |"
              << std::endl;
  }
}

void Renderer::RenderMenu() {
  ClearScreen();
  int w = 60;
  DrawDivider(w);
  std::cout << "| " << std::left << std::setw(w - 4) << "SHADOW INDEX"
            << " |" << std::endl;
  DrawDivider(w);
  std::cout << "| " << std::left << std::setw(w - 4) << " "
            << " |" << std::endl;
  std::cout << "| " << std::left << std::setw(w - 4) << "1. New Run"
            << " |" << std::endl;
  std::cout << "| " << std::left << std::setw(w - 4) << "2. Load Run"
            << " |" << std::endl;
  std::cout << "| " << std::left << std::setw(w - 4) << "3. Quit"
            << " |" << std::endl;
  std::cout << "| " << std::left << std::setw(w - 4) << " "
            << " |" << std::endl;
  DrawDivider(w);
  std::cout << "> ";
}

void Renderer::RenderGame(const ViewModel &vm) {
  ClearScreen();
  int w = 80;

  // Header
  DrawDivider(w);
  std::stringstream header;
  header << "SHADOW INDEX | Seed: " << vm.seed << " | Depth: " << vm.depth;
  std::cout << "| " << std::left << std::setw(w - 4) << header.str() << " |"
            << std::endl;
  DrawDivider(w);

  // Room Content
  std::cout << "| " << std::left << std::setw(w - 4) << vm.roomTitle << " |"
            << std::endl;
  std::cout << "| " << std::left << std::setw(w - 4) << " "
            << " |" << std::endl;

  for (const auto &line : vm.roomLines) {
    PrintWrapped(line, w);
  }
  std::cout << "| " << std::left << std::setw(w - 4) << " "
            << " |" << std::endl;
  DrawDivider(w);

  // Choices and Stats Split
  // For M1, let's just stack them to be simpler and safe on wrapping.
  // Stats section
  std::cout << "| STATS: ";
  std::cout << "RES: " << vm.stats.resolve << " ";
  std::cout << "DRD: " << vm.stats.dread << " ";
  std::cout << "CLA: " << vm.stats.clarity << " ";
  std::cout << "PRD: " << vm.stats.pride << " ";
  std::cout << "COM: " << vm.stats.compassion << " ";
  std::cout << std::setw(w - 4 - 45) << ""
            << " |" << std::endl; // filler

  if (!vm.activeShadows.empty()) {
    std::cout << "| SHADOWS: \n";
    for (const auto &s : vm.activeShadows) {
      std::cout << "|   * " << std::left << std::setw(w - 10) << s << "|\n";
    }
  }

  DrawDivider(w);

  // Draw Options
  std::cout << "\n";
  for (size_t i = 0; i < vm.options.size(); ++i) {
    std::cout << "  [" << (i + 1) << "] " << vm.options[i] << "\n";
  }

  // Draw Divider
  DrawDivider(w);

  // Draw Log
  if (!vm.log.empty()) {
    std::cout << "  LOG:\n";
    int start = std::max(0, (int)vm.log.size() - 3);
    for (int i = start; i < (int)vm.log.size(); ++i) {
      std::cout << "  > " << vm.log[i] << "\n";
    }
    DrawDivider(w);
  }

  std::cout << "  Choice: ";
}

void Renderer::RenderGameOver(const ViewModel &vm) {
  ClearScreen();
  DrawDivider(80);
  std::cout << "\n     Y O U  H A V E  F A D E D  I N T O  T H E  S H A D O W "
               ". . .\n\n";
  DrawDivider(80);
  std::cout << "  Final Depth: " << vm.depth << "\n";
  // Could show final stats here
  DrawDivider(80);
  std::cout << "  [Press Enter to return to Menu]\n";
}

void Renderer::RenderWin(const ViewModel &vm) {
  ClearScreen();
  DrawDivider(80);
  std::cout << "\n       T H E   S H A D O W   L I F T S . . .\n\n";
  std::cout << "           Y O U   H A V E   E S C A P E D .\n\n";
  DrawDivider(80);
  std::cout << "  Final Depth: " << vm.depth << "\n";
  std::cout << "  Resolve Remaining: " << vm.stats.resolve << "\n";
  DrawDivider(80);
  std::cout << "  [Press Enter to return to Menu]\n";
}
