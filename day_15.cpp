#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>

using namespace std;
using sensor = pair<long, long>;
using beacon = pair<long, long>;
using pos = pair<long, long>;
using posdist = pair<pos, long>;

long get_mdist(sensor s, beacon b) {
  return abs(s.first - b.first) + abs(s.second - b.second);
}

/// @brief Use geometry to figure out if give line would have non beacons
/// @param s sensor position
/// @param pos_map sensor, beacon, non beacon map
/// @param bdist beacon distance
/// @param y position of line to mark non beacons '#'
void mark_no_beacon(sensor s, map<pos, char> &pos_map, long bdist, long y) {
  // Where is sensor relative to y ?
  if (s.second < y) {
    // Sensor above
    if (s.second + bdist < y) {
      // sensor too far away
      cout << "Sensor above and too far away: " << s.first << "-" << s.second
           << endl;
      return;
    }

    // Find all possible left and right postions from sensors which
    // will drop vertical lines where there can be no beacons
    for (long i = 0; i <= bdist; i++) {
      if (s.second + bdist - i >= y) {
        if (pos_map.find({s.first - i, y}) == pos_map.end()) {
          pos_map[{s.first - i, y}] = '#';
        }
        if (pos_map.find({s.first + i, y}) == pos_map.end()) {
          pos_map[{s.first + i, y}] = '#';
        }
      } else {
        break;
      }
    }

  } else if (s.second > y) {
    // Sensor below
    if (s.second - bdist > y) {
      // sensor too far away
      cout << "Sensor below and too far away: " << s.first << "-" << s.second
           << endl;
      return;
    }

    for (long i = 0; i <= bdist; i++) {
      if (s.second - bdist + i <= y) {
        if (pos_map.find({s.first - i, y}) == pos_map.end()) {
          pos_map[{s.first - i, y}] = '#';
        }
        if (pos_map.find({s.first + i, y}) == pos_map.end()) {
          pos_map[{s.first + i, y}] = '#';
        }
      } else {
        break;
      }
    }
  } else {
    // On y line
    cout << "Sensor same line: " << s.first << "-" << s.second << endl;
    for (long i = 0; i <= bdist; i++) {
      if (pos_map.find({s.first - i, s.second}) == pos_map.end()) {
        pos_map[{s.first - i, s.second}] = '#';
      }
      if (pos_map.find({s.first + i, s.second}) == pos_map.end()) {
        pos_map[{s.first + i, s.second}] = '#';
      }
    }
  }
}

/// @brief  Breadth First to find all possible non beacon positions
/// @param s sensor location
/// @param dist dist from beacon
/// @param pos_map positions of sensors, beacons, non-beacons
void do_bfs(sensor s, long dist, map<pos, char> &pos_map) {
  // we can move 1 step up, down, left or right
  // each step is 1 unit dist
  queue<posdist> q;
  q.push({s, 0});
  set<pos> visit;
  visit.insert(s);
  vector<pos> dir = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

  while (!q.empty()) {
    posdist pd = q.front();
    auto cur_pos = pd.first;
    auto cur_dis = pd.second;
    q.pop();

    for (auto d : dir) {
      pos nbr{cur_pos.first + d.first, cur_pos.second + d.second};
      bool unvisited = (visit.find(nbr) == visit.end());
      bool empty_pos = false;
      if (pos_map.find(nbr) == pos_map.end()) {
        empty_pos = true;
      } else if (pos_map[nbr] == '#') {
        empty_pos = true;
      }
      if (unvisited and empty_pos and cur_dis + 1 <= dist) {
        visit.insert(nbr);
        q.push({nbr, cur_dis + 1});
        pos_map[nbr] = '#';
      }
    }
  }
}

int main() {
  string line;
  map<sensor, beacon> sb_map;
  while (getline(cin, line)) {
    stringstream ss(line);
    vector<long> positions;
    char c;
    while (ss >> c) {
      if (c == '=') {
        long p;
        ss >> p;
        positions.push_back(p);
      }
    }
    sb_map[{positions[0], positions[1]}] = {positions[2], positions[3]};
  }

  // If we need to find the positions where there are no beacons
  // we should traverse all the paths whose distance is <= beacon
  // This is because we are guaranteed that this is the only closest
  // beacon to the sensor

  // Create a map of all the sensors and beacons first
  map<pos, char> pos_map;
  for (auto &[k, v] : sb_map) {
    // Mark sensor and beacon
    pos_map[k] = 'S';
    pos_map[v] = 'B';
    assert(k != v);
  }
  long y_line = 2000000;
  for (auto &[s, b] : sb_map) {
    // if no beacon mark '#'
    long dist = get_mdist(s, b);
    mark_no_beacon(s, pos_map, dist, y_line);
  }

  long no_beacon{0};
  for (auto &[p, c] : pos_map) {
    if (p.second == y_line and c == '#') {
      no_beacon++;
    }
  }

  cout << "No beacon count " << no_beacon << endl;
  return 0;
}
