#include "Rng.hpp"

void Rng::Seed(int seed) { m_mt.seed(static_cast<unsigned int>(seed)); }

int Rng::Int(int min, int max) {
  if (min > max)
    return min;
  std::uniform_int_distribution<int> dist(min, max);
  return dist(m_mt);
}

float Rng::Float(float min, float max) {
  if (min >= max)
    return min;
  std::uniform_real_distribution<float> dist(min, max);
  return dist(m_mt);
}
