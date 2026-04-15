#pragma once
#include "student.h"

class MiddleStudent : public Student {
 public:
  MiddleStudent(std::string name) : Student(name) {}

  Solution solve(const Equation& ans) override;
};