#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Type of operations
enum op_type { MULT, ADD, MULTOLD };
// Construct our monkey with Q of starting items
class Monkey {
private:
  queue<long> startingItems;
  op_type operation;
  int op_value;
  int div_test;
  int div_test_true;
  int div_test_false;
  long long num_inspections{0};

public:
  Monkey(vector<int> sit, op_type op, int opv, int dt, int dt_true,
         int dt_false)
      : operation(op), op_value(opv), div_test(dt), div_test_true(dt_true),
        div_test_false(dt_false) {
    for (auto item : sit) {
      startingItems.push(item);
    }
  }
  int getInspections(void) { return num_inspections; }
  void grabItem(int worry) { startingItems.push(worry); }
  void takeTurn(vector<Monkey> &monkeys) {
    while (!startingItems.empty()) {
      num_inspections++;
      long oldw = startingItems.front();
      startingItems.pop();
      long neww;
      if (operation == MULT) {
        neww = oldw * op_value;
      } else if (operation == MULTOLD) {
        neww = oldw * oldw;
      } else {
        neww = oldw + op_value;
      }
      neww /= 3;

      if (neww % div_test == 0) {
        monkeys[div_test_true].grabItem(neww);
      } else {
        monkeys[div_test_false].grabItem(neww);
      }
    }
  }
};

int main() {
  Monkey monkey0({89, 84, 88, 78, 70}, MULT, 5, 7, 6, 7);
  Monkey monkey1({76, 62, 61, 54, 69, 60, 85}, ADD, 1, 17, 0, 6);
  Monkey monkey2({83, 89, 53}, ADD, 8, 11, 5, 3);
  Monkey monkey3({95, 94, 85, 57}, ADD, 4, 13, 0, 1);
  Monkey monkey4({82, 98}, ADD, 7, 19, 5, 2);
  Monkey monkey5({69}, ADD, 2, 2, 1, 3);
  Monkey monkey6({82, 70, 58, 87, 59, 99, 92, 65}, MULT, 11, 5, 7, 4);
  Monkey monkey7({91, 53, 96, 98, 68, 82}, MULTOLD, 11, 3, 4, 2);

  vector<Monkey> monkeys = {monkey0, monkey1, monkey2, monkey3,
                            monkey4, monkey5, monkey6, monkey7};

#if 0
  Monkey monkey0({79, 98}, MULT, 19, 23, 2, 3);
  Monkey monkey1({54, 65, 75, 74}, ADD, 6, 19, 2, 0);
  Monkey monkey2({79, 60, 97}, MULTOLD, 6, 13, 1, 3);
  Monkey monkey3({74}, ADD, 3, 17, 0, 1);
  vector<Monkey> monkeys = {monkey0, monkey1, monkey2, monkey3};
#endif

  for (int i = 0; i < 20; i++) {
    for (auto &m : monkeys) {
      m.takeTurn(monkeys);
    }
  }
  int i{0};
  vector <pair<int, int>> answer;   
  for (auto &m : monkeys) {
       answer.push_back({m.getInspections(), i++});
  }
  
  sort(answer.begin(), answer.end(), greater());
  for (auto& a : answer) {
     cout << "Monkey [" << a.second << "] performed " << a.first
         << " inspections" << endl;
  }
  cout << "Answer " << answer[0].first * answer[1].first << endl;
  return 0;
}