#include <iostream>
#include <string>
#include <set>
#include <sstream>

using namespace std;

int get_prio(char item) {
    if (isupper(item)) {
        return 27 + item - 'A';
    }

    return 1 + item - 'a';
}

int main () {
    string line;
    int priority{0};
    while (getline(cin, line)) {
        stringstream ss(line);
        string items;
        ss >> items;
        int len = items.length();
        set<char> dup;

        for (int i = 0; i < len / 2; i++) {
            dup.insert(items[i]);
        }

        for (int i = len / 2; i < len; i++) {
            if (dup.find(items[i]) != dup.end()) {
                priority += get_prio(items[i]);
                break;
            }
        }
        cout << "Priority is " << priority << endl;
    }
    return 0;
}