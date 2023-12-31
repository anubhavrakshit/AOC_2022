#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
using namespace std;

// Type is either (D)irectory or (F)ile
enum de_type { D, F };
struct dirent {
    string name;
    de_type type;
    int size;
};
using dirents = vector<dirent>;

int dfs(unordered_map<string, dirents> &index, string dir,
        unordered_map<string, int> &memo) {

    if (index.find(dir) == index.end()) {
        assert(0);
    }
    if (memo.find(dir) != memo.end()) {
        cout << "Cache Hit!!" << endl;
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

// Get the Current Working Dir path from
// pwd stack
string get_pwd_path(stack<string> pwd) {
    if (pwd.size() <= 1) {
        return "/";
    }

    string path;
    while (!pwd.empty()) {
        string curdir = pwd.top();
        pwd.pop();
        // For non root dir we will need
        // to prefix path seperator
        if (curdir == "/") {
            continue;
        } else {
            curdir = "/" + curdir;
            path = curdir + path;
        }
    }
    return path;
}

void parse(unordered_map<string, dirents> &index) {
    // Maintain a Present Working Dir in a stack.
    // Use this to determing the path for directory entries
    // Use this path as a key to store the directory entries
    // Keep this in an map called index
    string line;
    stack<string> pwd;
    while (getline(cin, line)) {
        stringstream ss(line);
        string tok;
        while (ss >> tok) {
            // All commands are prefixed by $
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
                    // Nothing to do here
                } else {
                    // We only know how to handle
                    // ls and cd
                    assert(0);
                }
            } else {
                // Must be output of ls command
                // Parse the directory entries
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
                string cwd = get_pwd_path(pwd);
                // Store the path -> dirent in index
                index[cwd].push_back({name, type, size});
            }
        }
    }
}

int main() {
    unordered_map<string, dirents> index;
    parse(index);
    for (auto &kv : index) {
        cout << "Dir " << kv.first << endl;
        for (auto &de : kv.second) {
            cout << "- " << de.name << " - " << de.size << endl;
        }
    }

    // Run a depth first search on "/" to store all the
    // directory sizes.
    unordered_map<string, int> memo;
    dfs(index, "/", memo);

    int num_dir_siz{0};
    int total_siz{0};
    for (auto kv : memo) {
        cout << kv.first << " - " << kv.second << endl;
        if (kv.second <= 100000) {
            num_dir_siz++;
            total_siz += kv.second;
        }
    }
    cout << "Sum of total sizes " << total_siz << endl;
    return 0;
}