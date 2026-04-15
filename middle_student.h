#pragma once
#include "student.h"

class MiddleStudent : public Student {
 public:
  MiddleStudent(const std::string& name) : Student(name) {}

  Solution solve(const Equation& ans) override;
};