#pragma once

class Solution {
 public:
  int rootsCount;
  double x1, x2;

  Solution(int count = 0, double r1 = 0, double r2 = 0)
      : rootsCount(count), x1(r1), x2(r2) {}

  ~Solution() {}
};