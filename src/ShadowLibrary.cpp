#include "ShadowLibrary.hpp"

void ShadowLibrary::InitSampleShadows() {
  m_shadows.clear();
  m_idToIndex.clear();

  auto Add = [&](const Shadow &s) {
    m_idToIndex[s.id] = m_shadows.size();
    m_shadows.push_back(s);
  };

  // 1. Shadow Claws
  Add({
      "shadow_claws",
      "Shadow Claws",
      "Your fingers end in smoky talons.",
      {1, 1, 0, 0, 0}, // +1 Resolve, +1 Dread
      {}               // No triggers
  });

  // 2. Whispering Stone
  Add({
      "whispering_stone",
      "Whispering Stone",
      "A stone that murmurs secrets when you enter new places.",
      {0, 0, 0, 0, 0},
      {{TriggerType::ON_ENTER_ROOM, {0, 0, 1, 0, 0}, ""}} // +1 Clarity on enter
  });

  // 3. Iron Heart
  Add({"iron_heart",
       "Iron Heart",
       "Your heart beats with the weight of iron.",
       {2, 0, 0, 0, -1}, // +2 Resolve, -1 Compassion
       {}});

  // 4. Veil of Mist
  Add({"veil_mist",
       "Veil of Mist",
       "You are hard to pin down.",
       {0, -1, 0, 0, 0}, // -1 Dread
       {}});
}

const Shadow *ShadowLibrary::GetShadow(const std::string &id) const {
  auto it = m_idToIndex.find(id);
  if (it != m_idToIndex.end()) {
    return &m_shadows[it->second];
  }
  return nullptr;
}
