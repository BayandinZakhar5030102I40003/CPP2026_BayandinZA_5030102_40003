#include <cstdlib>
#include <random>

#include "middle_student.h"

Solution MiddleStudent::solve(const Equation& ans) {
  static std::mt19937 rng(std::random_device{}());
  static std::uniform_real_distribution<double> prob(0.0, 1.0);

  double chance = prob(rng);
  if (chance <= 0.5) {
    return ans.solve();
  } else {
    static std::uniform_int_distribution<int> dist(0, 9);
    return Solution(1, dist(rng), dist(rng));
  }
}