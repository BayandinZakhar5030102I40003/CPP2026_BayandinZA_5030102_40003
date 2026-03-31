#pragma once
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class Solution {
 public:
  int rootsCount;
  double x1, x2;

  Solution(int count = 0, double r1 = 0, double r2 = 0)
      : rootsCount(count), x1(r1), x2(r2) {}
};

class Equation {
 public:
  double a, b, c;

  Equation(double a, double b, double c) : a(a), b(b), c(c) {}
  Solution solve() const {
    double D = b * b - 4 * a * c;

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
};

class Student {
 protected:
  std::string name;

 public:
  Student(std::string name) : name(name) {}
  virtual Solution solve(const Equation& ans) = 0;

  std::string getName() const { return name; }

  virtual ~Student() {}
};

class Good_S : public Student {
 public:
  Good_S(std::string name) : Student(name) {}

  Solution solve(const Equation& ans) override { return ans.solve(); }
};

class Mid_S : public Student {
 public:
  Mid_S(std::string name) : Student(name) {}

  Solution solve(const Equation& ans) override {
    double chance = (double)std::rand() / RAND_MAX;
    if (chance < 0.5) {
      return ans.solve();
    } else {
      return Solution(1, std::rand() % 10);
    }
  }
};

class Bad_S : public Student {
 public:
  Bad_S(std::string name) : Student(name) {}

  Solution solve(const Equation& ans) override { return Solution(1, 10); }
};

class Letter {
 public:
  std::string studName;
  Equation func;
  Solution answer;

  Letter(std::string name, Equation fnc, Solution ans)
      : studName(name), func(fnc), answer(ans) {}

};


