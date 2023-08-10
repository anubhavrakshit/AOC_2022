#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using row = vector<int>;
using grid = vector<row>;

int get_scenic_score(grid &g, int r, int c) {
    int nc = g[0].size();
    int nr = g.size();

    // Look up
    int up_visible{0};
    for (int i = r - 1; i >= 0; i--) {
        up_visible++;
        if (g[r][c] <= g[i][c]) {
            break;
        }
    }

    // Look down
    int down_visible{0};
    for (int i = r + 1; i < nr; i++) {
        down_visible++;
        if (g[r][c] <= g[i][c]) {
            break;
        }
    }

    // Look left
    int left_visible{0};
    for (int j = c - 1; j >= 0; j--) {
        left_visible++;
        if (g[r][c] <= g[r][j]) {
            break;
        }
    }

    // Look right
    int right_visible{0};
    for (int j = c + 1; j < nc; j++) {
        right_visible++;
        if (g[r][c] <= g[r][j]) {
            break;
        }
    }

    return (up_visible * down_visible * right_visible * left_visible);
}

int main() {
    grid g;
    string line;
    int nc{0}, nr{0};
    while (getline(cin, line)) {
        row r;
        for (char h : line) {
            r.push_back(h - '0');
        }
        g.push_back(r);
    }

    nc = g[0].size();
    nr = g.size();

    int max_scenic_score{0};
    // Look at each internal tree of the grid
    for (int i = 1; i < nr - 1; i++) {
        for (int j = 1; j < nc - 1; j++) {
            int scenic_score = get_scenic_score(g, i, j);
            cout << "G[" << i << "]"
                 << "[" << j << "]"
                 << " : " << g[i][j] << " score " << scenic_score << endl;
            max_scenic_score = max(max_scenic_score, scenic_score);
        }
    }
    cout << "Max scenic score " << max_scenic_score << endl;
}