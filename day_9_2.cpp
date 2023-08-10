#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <vector>
using namespace std;

// Cartesian co-ordinate (x, y)
using pos = pair<int, int>;

// Single step movement of Head
void move_single(pos &head, char dir)
{
	if (dir == 'R')
	{
		head.first += 1;
	}
	else if (dir == 'L')
	{
		head.first -= 1;
	}
	else if (dir == 'U')
	{
		head.second += 1;
	}
	else
	{
		head.second -= 1;
	}
}

// Make Tail follow the Head
void follow(pos &head, pos &tail)
{
	// All possible closest neigbours
	vector<pos> nbrs = {{0, 0}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, -1}, {1, 1}, {-1, -1}, {-1, 1}};
	bool is_nbr{false};
	for (auto p : nbrs)
	{
		if ((head.first == tail.first + p.first) and (head.second == tail.second + p.second))
		{
			is_nbr = true;
			break;
		}
	}

	// Skip moving tail if we are closest nbr of head
	if (is_nbr)
	{
		return;
	}

	// On same vertical line
	if (head.first == tail.first)
	{
		// Follow Up
		if (head.second > tail.second)
		{
			tail.second++;
		}
		else
		{
			tail.second--;
		}
	}
	else if (head.second == tail.second)
	{
		// On same horizontal line
		// Follow Right
		if (head.first > tail.first)
		{
			tail.first++;
		}
		else
		{
			tail.first--;
		}
	}
	else
	{
		// Figure out which Quadrant to move
		if (head.first > tail.first)
		{ // Towards right
			if (head.second > tail.second)
			{ // Up
				tail.first += 1;
				tail.second += 1;
			}
			else
			{
				tail.first += 1;
				tail.second -= 1;
			}
		}
		else
		{ // Left
			if (head.second > tail.second)
			{ // Up
				tail.first -= 1;
				tail.second += 1;
			}
			else
			{
				tail.first -= 1;
				tail.second -= 1;
			}
		}
	}
}

int main()
{
	// 0 index is for Head and rest are Knot 1, 2, 3 ...9
	// We will store positions of each Knot and Head in this
	// array
	vector<pos> all_knots(10);
	// Initial co-ordinate  is (0, 0)
	for (int i = 0; i < 10; i++)
	{
		all_knots[i] = {0, 0};
	}

	string line;
	set<pos> visit;
	while (getline(cin, line))
	{
		stringstream ss(line);
		char dir;
		int nsteps;
		ss >> dir >> nsteps;
		assert(nsteps > 0);
		while (nsteps--)
		{
			pos &head = all_knots[0];
			move_single(head, dir);
			// Make each knot follow the closest tail
			for (int i = 1; i < 10; i++)
			{
				follow(all_knots[i - 1], all_knots[i]);
			}
			visit.insert(all_knots[9]);
		}
	}
	cout << "Unique visit " << visit.size() << endl;
	return 0;
}