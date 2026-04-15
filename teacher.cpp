#include "teacher.h"

#include <cmath>
#include <fstream>
#include <iostream>

bool Teacher::Correct(const Solution& correct, const Solution& student) {
  const double EPS = 1e-6;

  if (correct.rootsCount != student.rootsCount) return false;

  if (correct.rootsCount == 1) return std::fabs(correct.x1 - student.x1) < EPS;

  if (correct.rootsCount == 2)
    return (std::fabs(correct.x1 - student.x1) < EPS &&
            std::fabs(correct.x2 - student.x2) < EPS) ||
           (std::fabs(correct.x1 - student.x2) < EPS &&
            std::fabs(correct.x2 - student.x1) < EPS);

  return true;
}

void Teacher::addSubmission(const Letter& sub) { queue.push_back(sub); }

void Teacher::checkAll() {
  for (const auto& sub : queue) {
    results[sub.studName] = 0;
  }

  for (const auto& sub : queue) {
    Solution correct = sub.func.solve();

    if (Correct(correct, sub.answer)) {
      results[sub.studName]++;
    }
  }
}

void Teacher::printResults() {
  for (const auto& p : results) {
    std::cout << p.first << " - " << p.second << std::endl;
  }
}

void Teacher::saveResultsToCSV(const std::string& filename) {
  std::ofstream file(filename);
  if (file.is_open()) {
    for (const auto& p : results) {
      file << p.first << ";" << p.second << "\n";
    }
    file.close();
  }
}