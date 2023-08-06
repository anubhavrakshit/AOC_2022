#include <iostream>
#include <string>
#include <sstream>

using namespace std;
using row = vector<int>;
using grid = vector<row>;

int is_visible(grid& g, int r, int c) {
    int nc = g[0].size();
    int nr = g.size();

    // Look up
    bool is_tallest {true};
    for (int i = r - 1; i >= 0; i--) {
        if (g[r][c] <= g[i][c]) {
            is_tallest = false;
            break;
        }
    }
    if (is_tallest) {
        return 1;
    }

    is_tallest = true;
    // Look down
    for (int i = r + 1; i < nr; i++) {
         if (g[r][c] <= g[i][c]) {
            is_tallest = false;
            break;
        }
    }
    if (is_tallest) {
        return 1;
    }
    
    // Look left
    is_tallest = true;
    for (int j = c - 1; j >= 0; j--) {
        if (g[r][c] <= g[r][j]) {
            is_tallest = false;
            break;
        }
    }
    if (is_tallest) {
        return 1;
    }
    // Look right
    is_tallest = true;
    for (int j = c + 1; j < nc; j++) {
        if (g[r][c] <= g[r][j]) {
            is_tallest = false;
            break;
        }
    }
    if (is_tallest) {
        return 1;
    }

    return 0;
}

int main() {
    grid g;
    string line;
    int nc{0}, nr{0};
    while (getline( cin, line)) {
        row r;
        for (char h : line) {
            r.push_back(h - '0');
        }
        g.push_back(r);
    }

    nc = g[0].size();
    nr = g.size();

    // Total number of boundary trees
    int count_visible = 2 * (nc + nr) - 4;

    // Look at each internal tree of the grid 
    for (int i = 1; i < nr - 1; i++) {
        for (int j = 1; j < nc - 1; j++) {
            count_visible += is_visible(g, i, j);
        }
    }
    cout << "Total visible " << count_visible << endl;
}