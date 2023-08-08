#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <vector>
using namespace std;

using pos = pair<int, int>;
void move_single(pos& head, char dir) {
    if (dir == 'R') {
        head.second += 1;
    } else if (dir == 'L') {
        head.second -= 1;
    } else if (dir == 'U') {
        head.first += 1;
    } else {
        head.first -= 1;
    }
}

void follow(pos& head, pos& tail) {
    vector<pos> nbrs = {{0,0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
    bool is_nbr{false};
    for (auto p : nbrs) {
        if ((head.first == tail.first + p.first) and (head.second == tail.second + p.second)) {
            is_nbr = true;
            break;
        }
    }

    if (is_nbr) {
        return;
    }
    
    if (head.first == tail.first) {
        if (head.second > tail.second) {
            tail.second++;
        } else {
            tail.second--;
        }
    } else if (head.second == tail.second) {
        if (head.first > tail.first) {
            tail.first++;
        } else {
            tail.first--;
        }
    } else {
        if (head.first > tail.first) { // Towards right
            if (head.second > tail.second) { // Up
                tail.first += 1;
                tail.second += 1;
            } else {
                tail.first += 1;
                tail.second -= 1;
            }
        } else { // Left
            if (head.second > tail.second) { // Up
                tail.first -= 1;
                tail.second += 1;
            } else {
                tail.first -= 1;
                tail.second -= 1;
            }
        }
    }
}
int main() {
    string line;
    set<pos> visit;
    pos head, tail;
    head = {0, 0};
    tail = {0, 0};
    visit.insert(tail);
    while (getline(cin, line)) {
        stringstream ss(line);
        char dir;
        int nsteps;
        ss >> dir >> nsteps;
        assert(nsteps > 0);
        while (nsteps--) {
            move_single(head, dir);
            follow(head, tail);
            visit.insert(tail);
        }
    }
    cout << "Unique visit " << visit.size() << endl;
    return 0;
}