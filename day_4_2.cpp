#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {

    string line;
    int num_overlaps{0};
    while (getline(cin, line)) {
        stringstream ss(line);
        int l1, h1, l2, h2;
        char dash, comma;
        ss >> l1 >> dash >> h1 >> comma >> l2 >> dash >> h2;
        cout << "Range" << l1 << "-" << h1 << "," << l2 << "-" << h2 << endl;
        if (l1 <= l2 and l2 >= l1 and l2 <= h1) {
            num_overlaps++;
        } else if (l2 <= l1 and l1 >= l2 and l1 <= h2) {
            num_overlaps++;
        }
    }
    cout << "Num Overlaps " << num_overlaps << endl;
    return 0;
}
