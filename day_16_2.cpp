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
// We use int ids to help in creating a key
// this key will be used for memoization
struct valve {
  size_t _id{0};      // Give unique int id
  size_t _open_id{0}; // Give unique open id
  string _name;
  int _flow_rate;
  vector<string> _nbrs;
  valve() = default;
  valve(string n, int f_rate, vector<string> nbrs)
      : _name(n), _flow_rate(f_rate), _nbrs(nbrs) {}
};

std::unordered_map<string, valve> all_valves;

// Return the unique open id for each valve
// Only a few valves have non zero rates.
// We only need to add these to the bitmap
size_t valve_to_oid(const string v) { return all_valves[v]._open_id; }

// Insert to opened bitmap
long long opened_ins(long long opened, int valve_id) {
  opened ^= (1 << valve_id);
  return opened;
}

bool is_open(long long opened, const string v) {
  auto valve_id = valve_to_oid(v);
  if (opened & (1 << valve_id)) {
    return true;
  }
  return false;
}

// Create key that will be inserted to memo object
long long create_key(const string &v, int time_left, long opened, int helpers) {
  long long key{0};
  key |= all_valves[v]._id;
  key |= (time_left << 16);
  key |= (helpers << 24);
  key |= (opened << 32);
  return key;
}

int solve(const string &v, int time_left, long long opened,
          unordered_map<long long, int> &memo, int helpers) {
  assert(time_left >= 0);
  if (time_left == 0) {
    // If we have helpers let them also explore once you have
    // finished.
    if (helpers) {
      return solve("AA", 26, opened, memo, helpers - 1);
    } else {
      return 0;
    }
  }

  if (memo.count(create_key(v, time_left, opened, helpers))) {
    return memo[create_key(v, time_left, opened, helpers)];
  }
  int ans{0};
  // Choices we have
  // 1. You Open the valve at this pos
  // 2. You explore the nbrs
  auto &cur_valve = all_valves[v];
  if (cur_valve._flow_rate and !is_open(opened, v)) {
    ans = (time_left - 1) * cur_valve._flow_rate +
          solve(cur_valve._name, time_left - 1,
                opened_ins(opened, cur_valve._open_id), memo, helpers);
  }
  for (const auto &nbr : cur_valve._nbrs) {
    ans = max(ans, solve(nbr, time_left - 1, opened, memo, helpers));
  }
  memo[create_key(v, time_left, opened, helpers)] = ans;
  return ans;
}

int main() {
  string line, start_valve;
  // Memoize the result of solve for given key
  unordered_map<long long, int> memo;
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

  // Store unique id for each valve
  // Used when we create keys to store in
  // memo object
  int i{1}, j{1};
  for (auto &[name, v] : all_valves) {
    if (v._flow_rate) {
      v._open_id = i++;
    }
    v._id = j++;
    std::cout << name << " : " << v._id << std::endl;
  }

  // Only one helper which is the elephant
  int time_left{26}, helpers{1};
  // Bitmap of currently open valve
  long long opened{0};
  auto ans = solve("AA", time_left, opened, memo, helpers);
  std::cout << "Max flow " << ans << std::endl;
  return 0;
}