#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void update_strength(int &sig_strength, int cycle, int X) {
	static int c = 20;
	if (cycle == c) {
		sig_strength +=  (cycle * X);
		cout << "---- " << sig_strength << endl;
		c += 40;
	}
}

int main() {
	string line;
	int cycle{1};
	string cmd;
	int V;
	int sig_strength{0};
	int X{1};
	while (getline(cin, line)) {
		stringstream ss(line);
		ss >> cmd;
		if (cmd == "addx") {
			ss >> V;
		} 

		cout << "Cycle " << cycle << "Cmd " << cmd << " X " << X << " Strength " << sig_strength << endl;
		update_strength(sig_strength, cycle, X);
		if (cmd == "noop") {
			cycle++;
			continue;
		} else {
			cycle++; // 1st cycle for add
			update_strength(sig_strength, cycle, X);
			cycle++; // 2nd cycle for add
			X += V; // Now update the register
		}
	}
	cout << "Total Sig Strength " << sig_strength << endl;
	return 0;
}