#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;
bool no_dups(unordered_map<char, int> &win_count) {
    for (pair<char, int> elems : win_count) {
        if (elems.second > 1) {
            return false;
        }
    }
    return true;
}
int main() {
    string line;
    const int win_size{4};
    while (getline(cin, line)) {
        int l{0}, r{0};
        unordered_map<char, int> win_count;
        while (r < line.length()) {
            if (r < win_size) {
                win_count[line[r]]++;
                r++;
            } else {
                if (no_dups(win_count)) {
                    cout << r << endl;
                    break;
                }
                win_count[line[l]]--;
                l++;
                win_count[line[r]]++;
                r++;
            }
        }
    }
    return 0;
}