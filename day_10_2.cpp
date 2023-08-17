#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using row = vector<char>;
using grid = vector<row>;

void update_crt(grid &crt, int cycle, int X) {
  int pixel_position = (cycle - 1) % 240;
  bool overlap{false};

  int row = pixel_position / 40;
  int col = pixel_position % 40;

  // We only worry about overlapping of col position
  // as the sprite only moves horizontally
  if (X - 1 == col or X == col or X + 1 == col) {
    overlap = true;
  }
  if (overlap) {
    crt[row][col] = '#';
  } else {
    crt[row][col] = '.';
  }
}

int main() {
  string line;
  int cycle{1};
  string cmd;
  int V;
  int X{1};
  grid crt(6, row(40)); // 6 rows each 40 col wide
  
  // Simulate a CPU by calling update_crt at each cycle
  while (getline(cin, line)) {
    stringstream ss(line);
    ss >> cmd;
    if (cmd == "addx") {
      ss >> V;
    }

    update_crt(crt, cycle, X);
    if (cmd == "noop") {
      cycle++;
      continue;
    } else {
      cycle++; // 1st cycle for add
      update_crt(crt, cycle, X);
      cycle++; // 2nd cycle for add
      X += V;  // Now update the register
    }
  }

  for (auto &r : crt) {
    for (auto pixel : r) {
      cout << pixel;
    }
    cout << endl;
  }
  return 0;
}