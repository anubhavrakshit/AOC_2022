#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Packet {
  vector<Packet> pkts;
  std::optional<int> element{std::nullopt};

  Packet() {}
  Packet(int data) { element = data; }
  Packet(const string &line) {
    size_t pos = 0;
    parse(line.substr(1, line.length() - 2), pos);
  }

  void parse(const string &line, size_t &pos) {
    while (pos < line.length()) {
      if (line[pos] == '[') {
        pos++;
        Packet p;
        p.parse(line, pos);
        pkts.push_back(p);
      } else if (line[pos] == ',') {
        pos++;
        continue;
      } else if (line[pos] == ']') {
        pos++;
        break;
      } else {
        auto start = pos;
        while (line[pos] >= '0' and line[pos] <= '9') {
          pos++;
        }
        auto data = stoi(line.substr(start, pos - start));
        Packet p(data);
        pkts.push_back(p);
      }
    }
  }

  void print(void) const {
    if (element.has_value()) {
      cout << element.value();
      return;
    }

    cout << "[";
    for (auto p : pkts) {
        p.print();
        cout << ",";
    }
    cout << "]";
    
  }
};

int compare(const Packet& left, const Packet& right) {
    bool left_is_list = !left.element.has_value();
    bool right_is_list = !right.element.has_value();

    if (left.element.has_value() and right.element.has_value()) {
        if (left.element.value() > right.element.value()) {
            return -1; // Bad order
        } else if(left.element.value() == right.element.value()) {
            return 0; // Check further
        } else {
            return 1; // Good order
        }
    } else if (left_is_list and right_is_list) {
        int left_len = left.pkts.size();
        int right_len = right.pkts.size();

        for (int i = 0; i < min(left_len, right_len); i++) {
            auto ret = compare(left.pkts[i], right.pkts[i]);
            if (ret == -1) {
                return -1;
            } else if (ret == 1) {
                return 1;
            }
        }
        if (left_len < right_len) {
            return 1;
        } else if (right_len == left_len) {
            return 0;
        } else {
            return -1;
        }
    } else if (left_is_list and !right_is_list) {
        int rval = right.element.value();
        string dummy = "[" + to_string(rval) + "]";
        Packet new_right(dummy);
        return compare(left, new_right);
    } else if (!left_is_list and right_is_list) {
        int lval = left.element.value();
        string dummy = "[" + to_string(lval) + "]";
        Packet new_left(dummy);
        return compare(new_left, right);
    }

    assert(0);
    return -1;
}

int main() {
  string line;
  int index{1};
  int ans{0};
  while (getline(cin, line)) {
    if (line.length()) {
      Packet p1_pkt(line);
      getline(cin, line);
      Packet p2_pkt(line);
      p1_pkt.print();
      cout << endl;
      p2_pkt.print();
      cout << endl;
      cout << endl;
      auto ret = compare(p1_pkt, p2_pkt);
      if (ret == 1) {
        ans += index;
      }
      index++;
    }
  }
  cout << "Ans = " << ans << endl;
  return 0;
}