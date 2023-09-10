#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;
using sensor = pair<int, int>;
using beacon = pair<int, int>;
using pos = pair<int, int>;
using posdist = pair<pos, int>;
using range = pair<int, int>;

int get_mdist(sensor s, beacon b) {
  return abs(s.first - b.first) + abs(s.second - b.second);
}

pair<int, int> range_nb(sensor s, int bdist, int y, int bound) {
  pair<int, int> range{-1, -1};
  // Where is sensor relative to y ?
  if (s.second < y and s.second + bdist < y) {
    // Sensor above
    // sensor too far away
    // cout << "Sensor above and too far away: " << s.first << "-" << s.second
    //     << endl;
    return range;
  } else if (s.second > y and s.second - bdist > y) {
    // Sensor below
    // sensor too far away
    // cout << "Sensor below and too far away: " << s.first << "-" << s.second
    //     << endl;
    return range;
  }
  // Vertical distance of 'y' line from Sensor
  auto y2 = abs(y - s.second);
  pos left_bound = {s.first - bdist + y2, y};
  pos right_bound = {s.first + bdist - y2, y};

  assert(left_bound.first <= right_bound.first);
  if (right_bound.first < 0) {
    return range;
  }

  if (left_bound.first > bound) {
    return range;
  }
  left_bound.first = max(left_bound.first, 0);
  right_bound.first = min(right_bound.first, bound);
  return {left_bound.first, right_bound.first};
}

/// @brief How many points on line 'y' have no beacons
/// @param s sensor position
/// @param y_map the map of "S", "B", "#"
/// @param bdist the manhattan dist of beacon from sensor
/// @param y the position of horizontal line where we are checking for beacon
/// @return count of no beaon points
int get_nb_count(sensor s, unordered_map<int, char> &y_map, int bdist, int y) {
  int num_nb{0};
  // Where is sensor relative to y ?
  if (s.second < y and s.second + bdist < y) {
    // Sensor above
    // sensor too far away
    // cout << "Sensor above and too far away: " << s.first << "-" << s.second
    //     << endl;
    return num_nb;
  } else if (s.second > y and s.second - bdist > y) {
    // Sensor below
    // sensor too far away
    // cout << "Sensor below and too far away: " << s.first << "-" << s.second
    //     << endl;
    return num_nb;
  }
  // Vertical distance of 'y' line from Sensor
  auto y2 = abs(y - s.second);
  pos left_bound = {s.first - bdist + y2, y};
  pos right_bound = {s.first + bdist - y2, y};

  for (int i = left_bound.first; i <= right_bound.first; i++) {
    if (y_map.find(i) == y_map.end()) {
      num_nb++;
      y_map[i] = '#';
    }
  }

  return num_nb;
}

bool is_complete_range(vector<range> ranges, int bound) {
  sort(ranges.begin(), ranges.end());
  stack<range> merged;
  merged.push(ranges[0]);

  for (int i = 1; i < ranges.size(); i++) {
    auto top = merged.top();
    auto cur = ranges[i];
    if (top.second < cur.first) {
      merged.push(cur);
    } else if (top.second < cur.second) {
      merged.pop();
      top.second = cur.second;
      merged.push(top);
    }
  }

  while (!merged.empty()) {
    auto r = merged.top();
    merged.pop();
    if (r.first == 0 and r.second == bound) {
      return true;
    }
  }
  return false;
}

int main() {
  string line;
  map<sensor, beacon> sb_map;
  while (getline(cin, line)) {
    stringstream ss(line);
    vector<int> positions;
    char c;
    while (ss >> c) {
      if (c == '=') {
        int p;
        ss >> p;
        positions.push_back(p);
      }
    }
    sb_map[{positions[0], positions[1]}] = {positions[2], positions[3]};
  }

  // Create a map at y line
  auto create_y_map = [&](int y) {
    unordered_map<int, char> y_map;
    for (auto &[k, v] : sb_map) {
      // Mark sensor and beacon
      if (k.second == y) {
        y_map[k.first] = 'S';
      }
      if (v.second == y) {
        y_map[v.first] = 'B';
      }
      assert(k != v);
    }
    return y_map;
  };

  bool part1 = false;
  if (part1) {
    // Part 1
    int y = 2000000;
    auto y_map = create_y_map(y);
    int nb_count = 0;
    for (auto &[s, b] : sb_map) {
      // If we need to find the positions where there are no beacons
      // we should traverse all the paths whose distance is <= beacon
      // This is because we are guaranteed that this is the only closest
      // beacon to the sensor
      auto bdist = get_mdist(s, b);
      nb_count += get_nb_count(s, y_map, bdist, y);
    }
    cout << "Ans is " << nb_count << endl;
  } else {
    // We are only scanning for postions {0, 0} - {4000000,4000000}
    int max_x = 4000000, max_y = 4000000;
    // For each line at 'y' dist figure out the ranges where
    // there can be no beacons. Then merge the ranges and check if we
    // have a merged range that covers the entire line at 'y'
    for (int y = 0; y <= max_y; y++) {
      vector<range> all_ranges;
      // O(s)  s - sensor count
      for (auto &[s, b] : sb_map) {
        auto bdist = get_mdist(s, b);
        // Figure out the range where there are no beacons on line 'y'
        // O(1)
        auto range = range_nb(s, bdist, y, max_x);
        if (range.first != -1 and range.second != -1) {
          all_ranges.push_back(range);
        }
      }
      // Merge all the ranges
      // O(nr) num of ranges
      if (!is_complete_range(all_ranges, max_x)) {
        auto y_map = create_y_map(y);
        cout << "Found beacon line at " << y << endl;
        for (auto &[s, b] : sb_map) {
          auto bdist = get_mdist(s, b);
          get_nb_count(s, y_map, bdist, y);
        }

        // Scan entire line y for the exact position where
        // there can be a beacon
        for (int i = 0; i <= max_x; i++) {
          if (y_map.find(i) == y_map.end()) {
            long tuning_freq = (long)i * (long)4000000 + (long)y;
            cout << "Tuning freq is " << tuning_freq << " at " << i << ":" << y
                 << endl;
            return 0;
          }
        }
      }
    }
  }
  return 0;
}
