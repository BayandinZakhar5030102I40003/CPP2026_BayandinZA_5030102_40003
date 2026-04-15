#include "equation.h"

#include <cmath>

Solution Equation::solve() const {
  double D = b * b - 4 * a * c;

  if (a == 0) {
    if (b == 0) {
      return Solution(0);
    }
    double x = -c / b;
    return Solution(1, x, x);
  }

  if (D > 0) {
    double x1 = (-b + sqrt(D)) / (2 * a);
    double x2 = (-b - sqrt(D)) / (2 * a);
    return Solution(2, x1, x2);
  } else if (D == 0) {
    double x = -b / (2 * a);
    return Solution(1, x, x);
  }
  return Solution(0);
}