#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using rockpos = pair<int, int>;
using rockline = vector<rockpos>;

void mark_in_map(set<rockpos> &block_map, vector<rockline> &all_rlines) {
  for (auto &rline : all_rlines) {
    rockpos prev = rline[0];
    block_map.insert(prev);
    for (int i = 1; i < rline.size(); i++) {
      rockpos next = rline[i];
      int x1 = prev.first;
      int y1 = prev.second;
      int x2 = next.first;
      int y2 = next.second;
      // Up
      if (x1 == x2 and y2 < y1) {
        for (int i = y2; i <= y1; i++) {
          block_map.insert({x1, i});
        }
      }
      // Down
      else if (x1 == x2 and y2 > y1) {
        for (int i = y1; i <= y2; i++) {
          block_map.insert({x1, i});
        }
      }
      // Left
      else if (y1 == y2 and x2 < x1) {
        for (int i = x2; i <= x1; i++) {
          block_map.insert({i, y1});
        }
      }
      // Right
      else if (y1 == y2 and x2 > x1) {
        for (int i = x1; i <= x2; i++) {
          block_map.insert({i, y1});
        }
      } else {
        assert(0);
      }
      prev = next;
    }
  }
}

int main() {
  string line;
  vector<rockline> all_rlines;
  int deepest_y_pos{0};
  while (getline(cin, line)) {
    stringstream ss(line);
    int x, y;
    char comma;
    rockline rl;
    while (ss >> x >> comma >> y) {
      deepest_y_pos = max(deepest_y_pos, y);
      rl.push_back({x, y});
      if (ss.peek() == ' ') {
        string arrow;
        ss >> arrow;
        continue;
      }
      break;
    }
    all_rlines.push_back(rl);
  }

  for (auto &rl : all_rlines) {
    for (auto &[rpos_x, rpos_y] : rl) {
      cout << rpos_x << "," << rpos_y << "->";
    }
    cout << endl;
  }

  set<rockpos> block_map;
  mark_in_map(block_map, all_rlines);
  rockpos fillpos{500, 0};
  bool is_forever{false};

  int sand_num{0};
  while (true) {
    rockpos cur_pos = fillpos;
    rockpos down = {0, 1};
    rockpos left = {-1, 0};
    rockpos right = {2, 0};
    cout << "Sand num " << sand_num << endl;
    while (block_map.find(cur_pos) == block_map.end()) {
      cout << "Cur pos " << cur_pos.first << ":" << cur_pos.second << endl;
      if (cur_pos.second > deepest_y_pos) {
        is_forever = true;
        break;
      }

      auto saved_pos = cur_pos;
      // Go down on step
      cur_pos.first += down.first;
      cur_pos.second += down.second;
      // If blocked move left
      if (block_map.find(cur_pos) != block_map.end()) {
        cur_pos.first += left.first;
        // If blocked move right
        if (block_map.find(cur_pos) != block_map.end()) {
          cur_pos.first += right.first;
          cur_pos.second += right.second;
          // Completely blocked
          if (block_map.find(cur_pos) != block_map.end()) {
            block_map.insert(saved_pos);
            break;
          }
        }
      }
    }

    if (is_forever) {
      break;
    }
    sand_num++;
  }
  cout << "Ans is " << sand_num << endl;

  return 0;
}