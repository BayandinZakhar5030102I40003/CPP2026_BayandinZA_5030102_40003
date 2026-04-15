#pragma once
#include <string>

#include "equation.h"

class Student {
 protected:
  std::string name;

 public:
  Student(std::string studName) : name(studName) {}
  virtual Solution solve(const Equation& ans) = 0;

  std::string getName() const { return name; }

  virtual ~Student() {}
};