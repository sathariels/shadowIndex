#ifndef RNG_HPP
#define RNG_HPP

#include <random>

class Rng {
public:
  void Seed(int seed);

  // Returns random int in [min, max] inclusive
  int Int(int min, int max);

  // Returns random float in [min, max] inclusive
  float Float(float min, float max);

private:
  std::mt19937 m_mt;
};

#endif // RNG_HPP
