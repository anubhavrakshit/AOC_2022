#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
using namespace std;
using std::set;
using std::tuple;

std::hash<std::string> hashfn;
struct valve {
  string _name;
  int _flow_rate;
  vector<string> _nbrs;
  valve() = default;
  valve(string n, int f_rate, vector<string> nbrs)
      : _name(n), _flow_rate(f_rate), _nbrs(nbrs) {}
};

std::unordered_map<string, valve> all_valves;
int solve(const string &v, int time_left, std::set<size_t> opened,
          map<tuple<string, int, std::set<size_t>>, int> &memo) {
  assert(time_left >= 0);
  if (time_left == 0) {
    return 0;
  }

  if (memo.count({v, time_left, opened})) {
    return memo[{v, time_left, opened}];
  }
  int ans{0};
  // Choices we have
  // 1. Open the valve at this pos
  // 2. Visit all nbrs and explore
  auto &cur_valve = all_valves[v];
  if (cur_valve._flow_rate and !opened.count(hashfn(v))) {
    opened.insert(hashfn(v));
    ans = (time_left - 1) * cur_valve._flow_rate +
          solve(cur_valve._name, time_left - 1, opened, memo);
    opened.erase(hashfn(v));
  }
  for (const auto &nbr : cur_valve._nbrs) {
    ans = max(ans, solve(nbr, time_left - 1, opened, memo));
  }
  memo[{v, time_left, opened}] = ans;
  return ans;
}

int main() {
  string line, start_valve;
  map<tuple<string, int, set<size_t>>, int> memo;
  while (getline(cin, line)) {
    stringstream ss(line);
    string tok, src, src_rate, nbr;
    while (ss >> tok) {
      if (tok == "Valve") {
        ss >> src;
        std::cout << "Valve = " << src;
      } else if (tok == "flow") {
        ss >> src_rate;
        src_rate = src_rate.substr(5);
        std::cout << " rate = " << src_rate;
      } else if (tok == "valves" or tok == "valve") {
        vector<string> nbrs;
        std::cout << " nbrs: ";
        while (ss >> nbr) {
          nbr = nbr.substr(0, 2);
          nbrs.emplace_back(nbr);
          std::cout << nbr << " ";
        }
        valve v(src, std::stoi(src_rate), nbrs);
        all_valves.insert({src, v});
        std::cout << std::endl;
      }
    }
  }

  int time_left{30};
  std::set<size_t> opened;
  auto ans = solve("AA", time_left, opened, memo);
  std::cout << "Max flow " << ans << std::endl;
  return 0;
}