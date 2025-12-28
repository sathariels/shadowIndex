#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "ViewModel.hpp"
#include <string>
#include <vector>

class Renderer {
public:
  void RenderMenu();
  void RenderGame(const ViewModel &vm);
  void RenderGameOver(const ViewModel &vm);
  void RenderWin(const ViewModel &vm);

private:
  void ClearScreen();
  // Helper to draw a horizontal line of a given width
  void DrawDivider(int width);
  // Helper to print text wrapped to a width
  void PrintWrapped(const std::string &text, int width,
                    const std::string &prefix = "");
};

#endif // RENDERER_HPP
