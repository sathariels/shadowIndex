#ifndef SHADOWLIBRARY_HPP
#define SHADOWLIBRARY_HPP

#include "Shadow.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class ShadowLibrary {
public:
  void InitSampleShadows();
  const Shadow *GetShadow(const std::string &id) const;

private:
  std::vector<Shadow> m_shadows;
  std::unordered_map<std::string, size_t> m_idToIndex;
};

#endif // SHADOWLIBRARY_HPP
