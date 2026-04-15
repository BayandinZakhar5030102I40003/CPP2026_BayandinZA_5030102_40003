#include "equation.h"

#include <cmath>

Solution Equation::solve() const {
  double D = b * b - 4 * a * c;
  const double EPS = 1e-6;

  if (fabs(a) < EPS) {
    if (fabs(b) < EPS) {
      return Solution(0);
    }
    double x = -c / b;
    return Solution(1, x, x);
  }

  if (D > EPS) {
    double x1 = (-b + sqrt(D)) / (2 * a);
    double x2 = (-b - sqrt(D)) / (2 * a);
    return Solution(2, x1, x2);
  } else if (D < EPS) {
    double x = -b / (2 * a);
    return Solution(1, x, x);
  }
  return Solution(0);
}