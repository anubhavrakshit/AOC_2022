#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    int max_cal_sofar{0}, current_cal{0};
    string input;
    vector<int> cals;
    while (getline(cin, input)) {
        if (!input.length()) {
            cals.push_back(current_cal);
            current_cal = 0;
            continue;
        }
        stringstream ss(input);
        int c;
        ss >> c;
        current_cal += c;
        max_cal_sofar = max(max_cal_sofar, current_cal);
    }
    // Last calorie value needs to be accounted for
    cals.push_back(current_cal);
    std::cout << "Max cal " << max_cal_sofar << endl;
    sort(cals.begin(), cals.end(), greater());
    std::cout << "Top 3 cal sum " << cals[0] + cals[1] + cals[2] << endl;
    return 0;
}