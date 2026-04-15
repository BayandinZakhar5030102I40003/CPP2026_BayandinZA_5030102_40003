#pragma once
#include "student.h"

class BadStudent : public Student {
 public:
  BadStudent(const std::string& name) : Student(name) {}

  Solution solve(const Equation& ans) override;
};