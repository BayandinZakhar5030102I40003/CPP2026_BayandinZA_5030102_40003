#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "bad_student.h"
#include "equation.h"
#include "good_student.h"
#include "middle_student.h"
#include "teacher.h"

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
      new MiddleStudent("Sergey"),
      new GoodStudent("Anton"),
      new MiddleStudent("Petr"),
      new BadStudent("Sasha"),
      new MiddleStudent("Timur"),
      new MiddleStudent("Timofei"),
      new BadStudent("Zakhar"),
      new MiddleStudent("Roman"),
      new GoodStudent("Rafael Ambrosius Kusto"),
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
  teacher.saveResultsToCSV("results.csv");

  for (auto s : students) {
    delete s;
  }

  return 0;
}