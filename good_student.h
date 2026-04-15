#pragma once
#include "student.h"

class GoodStudent : public Student {
 public:
  GoodStudent(std::string name) : Student(name) {}

  Solution solve(const Equation& ans) override;
};