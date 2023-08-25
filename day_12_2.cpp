#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

using row = vector<char>;
using grid = vector<row>;
using pos = pair<int, int>;

int do_bfs(grid &heightmap, pos startpos, pos endpos) {
  set<pos> visit;
  int min_dist{INT_MAX};
  // qentry will contain pos, distance
  using qentry = pair<pos, int>;
  queue<qentry> qe_queue;
  qe_queue.push({startpos, 0});
  int nrows = heightmap.size();
  int ncols = heightmap[0].size();

  visit.insert(startpos);
  while (!qe_queue.empty()) {
    auto entry = qe_queue.front();
    qe_queue.pop();
    auto entry_pos = entry.first;
    auto entry_dis = entry.second;

    // Reached Endpos
    if (entry_pos == endpos) {
      min_dist = entry_dis;
      break;
    }

    // Start Pos has height of a
    int curheight = heightmap[entry_pos.first][entry_pos.second];
    auto is_valid_pos = [&](int i, int j) {
      if (i < 0 or j < 0 or i >= nrows or j >= ncols) {
        return false;
      }

      int nextheight = heightmap[i][j];
      if (nextheight - curheight == 1 or curheight >= nextheight) {
        return true;
      }
      return false;
    };

    // Get all valid next positions
    vector<pair<int, int>> alldirs = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    for (auto &dir : alldirs) {
      pos next_pos = {entry_pos.first + dir.first,
                      entry_pos.second + dir.second};
      if (is_valid_pos(next_pos.first, next_pos.second) and
          !visit.count(next_pos)) {
        qe_queue.push({next_pos, entry_dis + 1});
        visit.insert(next_pos);
      }
    }
  }

  return min_dist;
}

int main() {
  grid heightmap;
  string line;
  while (getline(cin, line)) {
    stringstream ss(line);
    row r;
    for (auto c : line) {
      r.push_back(c);
    }
    heightmap.push_back(r);
  }

  int nrows = heightmap.size();
  int ncols = heightmap[0].size();
  cout << "Num rows : cols  " << nrows << " : " << ncols << endl;

  // Find the start pos
  vector<pos> all_startpos;
  pos endpos;
  int min_dis{INT_MAX};
  for (int i = 0; i < nrows; i++) {
    for (int j = 0; j < ncols; j++) {
      if (heightmap[i][j] == 'S' or heightmap[i][j] == 'a') {
        // height of S is a
        heightmap[i][j] = 'a';
        all_startpos.push_back({i, j});
      } else if (heightmap[i][j] == 'E') {
        heightmap[i][j] = 'z';
        endpos = {i, j};
      }
    }
  }

  for (auto startpos : all_startpos) {
    min_dis = min(do_bfs(heightmap, startpos, endpos), min_dis);
  }

  cout << "Min steps " << min_dis << endl;
  return 0;
}