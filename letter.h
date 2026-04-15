#pragma once

#include <string>

#include "equation.h"
#include "solution.h"

class Letter {
 public:
  std::string studName;
  Equation func;
  Solution answer;

  Letter(std::string name, Equation fnc, Solution ans)
      : studName(name), func(fnc), answer(ans) {}

  ~Letter() {}
};