#include <cstdlib>

#include "middle_student.h"

Solution MiddleStudent::solve(const Equation& ans) {
  double chance = (double)std::rand() / RAND_MAX;
  if (chance <= 0.5) {
    return ans.solve();
  } else {
    return Solution(1, std::rand() % 10, std::rand() % 10);
  }
}