#include <iostream>
#include <string>
#include <deque>
#include <sstream>

using namespace std;

int main() {

    string line;
    int num_overlaps{0};
    // Assume we have at max 10 stacks
    vector<deque<char>> st(10);
    while (getline(cin, line)) {
       if (line[0] != 'm') {
        for (int j = 0; j < line.length(); j += 4) {
            int pos = j / 4;
            if (line[j] == '[') {
                cout << "Push " << line[j + 1] << " to stack " << pos << endl;
                st[pos].push_front(line[j + 1]);
            }
        }
       } else if (line[0] == 'm') {
        int nops, src, dst;
        string mov, from, to;
        stringstream ss(line);
        ss >> mov >> nops >> from >> src >> to >> dst;
        cout << "Move " << nops << " from " << src << " -> " << dst << endl;
        src -= 1;
        dst -= 1;
        for (int i = 0; i < nops; i++) {
            auto popped = st[src].back();
            st[src].pop_back();
            st[dst].push_back(popped);
        }
       }
    }
    cout << "Print results " << endl;
    for (int i = 0; i < 10; i++) {
        if (!st[i].empty()) {
            cout << st[i].back();
        }
    }
    
    cout << endl;
    return 0;
}
