#include "Input.hpp"
#include <iostream>
#include <limits>

int Input::GetInt() {
  int val;
  if (!(std::cin >> val)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return -1;
  }
  // Consume newline
  // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return val;
}
