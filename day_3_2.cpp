#include <iostream>
#include <set>
#include <sstream>
#include <string>

using namespace std;

int get_prio(char item) {
    if (isupper(item)) {
        return 27 + item - 'A';
    }

    return 1 + item - 'a';
}

int main() {
    string line1;
    int priority{0};
    while (getline(cin, line1)) {
        string line2, line3;
        getline(cin, line2);
        getline(cin, line3);
        set<char> dup, dup12;
        for (auto c : line1) {
            dup.insert(c);
        }

        for (auto c : line2) {
            if (dup.find(c) != dup.end()) {
                dup12.insert(c);
            }
        }

        for (auto c : line3) {
            if (dup12.find(c) != dup12.end()) {
                priority += get_prio(c);
                cout << "Found " << c << endl;
                break;
            }
        }
    }
    cout << "Priority " << priority << endl;
    return 0;
}