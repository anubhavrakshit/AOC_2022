#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

enum de_type {D, F};
struct dirent {
    string name;
    de_type type;
    int size;
};
using dirents = vector<dirent>;

int dfs(unordered_map<string, dirents>& index, string dir, unordered_map<string, int>& memo) {
    
    cout << "Look up " << dir << endl;
    if (index.find(dir) == index.end()) {
        return 0;
    } 
    if (memo.find(dir) != memo.end()) {
        return memo[dir];
    }

    int dirsize{0};
    for (auto &dents : index[dir]) {
        if (dents.type == F) {
            dirsize += dents.size;
        } else {
            assert(dents.size == 0);
            string path;
            if (dir == "/") {
                path = dir + dents.name;
            } else {
                path = dir + "/" + dents.name;
            }
            dirsize += dfs(index, path, memo);
        }
    }
    memo[dir] = dirsize;
    return memo[dir];
} 
string get_cwd(stack<string> pwd) {
    if (pwd.size() <= 1) {
        return "/";
    }

    string path;
    while (!pwd.empty()) {
        string curdir = pwd.top();
        pwd.pop();
        if (curdir == "/") {
            continue;
        } else {
            curdir = "/" + curdir;
            path = curdir + path; 
        }
    }
    return path;
}
int main() {
    stack<string> pwd;
    unordered_map<string, dirents> index;
    const long total_available_space {70000000};
    const long required_free_space {30000000};

    string line;
    while (getline(cin, line)) {
        stringstream ss(line);
        string tok;
        while (ss >> tok) {
            if (tok == "$") {
                string cmd;
                ss >> cmd;
                if (cmd == "cd") {
                    string dir;
                    ss >> dir;
                    if (dir == "..") {
                        pwd.pop();
                    } else {
                        pwd.push(dir);
                    }
                } else if (cmd == "ls") {
                } else {
                    assert(0);
                }
            } else {
                de_type type;
                int size{0};
                string name;
                if (tok == "dir") {
                    type = D;
                } else {
                    type = F;
                    size = stoi(tok);
                }
                ss >> name;
                string cwd = get_cwd(pwd);
                cout << "CWD: " << cwd << endl;
                index[cwd].push_back({name, type, size});
            }
        }
    }

    for (auto& kv : index) {
        cout << "Dir " << kv.first << endl;
        for (auto& de : kv.second) {
            cout << "- " << de.name << " - " << de.size << endl;
        }
    }
    unordered_map<string, int> memo;
    int cur_used_space = dfs(index, "/", memo);
    int cur_free_space = total_available_space - cur_used_space;
    assert(cur_free_space < required_free_space);
    int to_del = required_free_space - cur_free_space;

    cout << "Need to delete atleast " << to_del << endl;
    vector<int> del_dir_sizes; 
    for (auto kv : memo) {
        cout << kv.first << " - " << kv.second << endl;
        if (kv.second >= to_del) {
            del_dir_sizes.push_back(kv.second);
        }
    }
    sort(del_dir_sizes.begin(), del_dir_sizes.end());
    cout << "Candidate " << del_dir_sizes[0] << endl;
    return 0;
}