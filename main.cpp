#pragma once
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
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

    if (a == 0) {
      if (b == 0) return Solution(0);
      double x = -c / b;
      return Solution(1, x, x);
    }

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
      return Solution(1, std::rand() % 10, std::rand() % 10);
    }
  }
};

class Bad_S : public Student {
 public:
  Bad_S(std::string name) : Student(name) {}

  Solution solve(const Equation& ans) override { return Solution(1, 10, 10); }
};

class Letter {
 public:
  std::string studName;
  Equation func;
  Solution answer;

  Letter(std::string name, Equation fnc, Solution ans)
      : studName(name), func(fnc), answer(ans) {}
};

class Teacher {
 private:
  std::vector<Letter> queue;
  std::map<std::string, int> results;

  bool isCorrect(const Solution& correct, const Solution& student) {
    const double EPS = 1e-6;

    if (correct.rootsCount != student.rootsCount) return false;

    if (correct.rootsCount == 1)
      return std::fabs(correct.x1 - student.x1) < EPS;

    if (correct.rootsCount == 2)
      return (std::fabs(correct.x1 - student.x1) < EPS &&
              std::fabs(correct.x2 - student.x2) < EPS) ||
             (std::fabs(correct.x1 - student.x2) < EPS &&
              std::fabs(correct.x2 - student.x1) < EPS);

    return true;
  }

 public:
  void addSubmission(const Letter& sub) { queue.push_back(sub); }

  void checkAll() {
    for (const auto& sub : queue) {
      results[sub.studName] = 0;
    }

    for (const auto& sub : queue) {
      Solution correct = sub.func.solve();

      if (isCorrect(correct, sub.answer)) {
        results[sub.studName]++;
      }
    }
  }

  void printResults() {
    for (const auto& p : results) {
      std::cout << p.first << " - " << p.second << std::endl;
    }
  }
};

int main() {
  std::srand(std::time(0));

  std::vector<Equation> equations;

  std::ifstream file("equations.txt");
  if (!file.is_open()) {
    std::cerr << "Erorr" << std::endl;
    return 1;
  }

  double a, b, c;
  while (file >> a >> b >> c) {
    equations.emplace_back(a, b, c);
  }
  file.close();

  std::vector<Student*> students = {
      new Mid_S("Sergey"), new Good_S("Anton"), new Mid_S("Petr"),
      new Bad_S("Sasha"),  new Mid_S("Timur"),  new Mid_S("Timofei"),
      new Bad_S("Zakhar"), new Mid_S("Roman"),  new Good_S("Rafael Ambrosius Kusto"),
  };

  Teacher teacher;

  for (auto student : students) {
    for (const auto& eq : equations) {
      Solution ans = student->solve(eq);
      teacher.addSubmission(Letter(student->getName(), eq, ans));
    }
  }

  teacher.checkAll();
  teacher.printResults();

  for (auto s : students) {
    delete s;
  }

  return 0;
}