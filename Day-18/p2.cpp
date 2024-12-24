/* The Historians aren't as used to moving around in this pixelated universe as
you are. You're afraid they're not going to be fast enough to make it to the
exit before the path is completely blocked.

To determine how fast everyone needs to go, you need to determine the first byte
that will cut off the path to the exit.

In the above example, after the byte at 1,1 falls, there is still a path to the
exit:

O..#OOO
O##OO#O
O#OO#OO
OOO#OO#
###OO##
.##O###
#.#OOOO
However, after adding the very next byte (at 6,1), there is no longer a path to
the exit:

...#...
.##..##
.#..#..
...#..#
###..##
.##.###
#.#....
So, in this example, the coordinates of the first byte that prevents the exit
from being reachable are 6,1.

Simulate more of the bytes that are about to corrupt your memory space. What are
the coordinates of the first byte that will prevent the exit from being
reachable from your starting position? (Provide the answer as two integers
separated by a comma with no other characters.)

Your puzzle answer was 60,46.*/

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Directions for movement
vector<pair<int, int>> dd = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
const int N = 70;

// Custom hash function for pair<int, int>
struct pair_hash {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2> &pair) const {
    return hash<T1>()(pair.first) ^ (hash<T2>()(pair.second) << 1);
  }
};

// Heuristic for A*
int h(int i, int j) { return abs(N - i) + abs(N - j); }

// Check if a position is within the grid and not in the obstacle set
bool in_grid(int i, int j, const vector<pair<int, int>> &coords, int idx) {
  return 0 <= i && i <= N && 0 <= j && j <= N &&
         find(coords.begin(), coords.begin() + idx, make_pair(i, j)) ==
             coords.begin() + idx;
}

// Check if the target is reachable with the first `idx` coordinates blocked
bool doable(const vector<pair<int, int>> &coords, int idx) {
  priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>>
      q;
  unordered_map<pair<int, int>, int, pair_hash> cost;

  q.push({h(0, 0), 0, 0});

  while (!q.empty()) {
    auto [c, i, j] = q.top();
    q.pop();

    pair<int, int> current = {i, j};
    if (cost.find(current) != cost.end())
      continue;
    cost[current] = c - h(i, j);

    if (current == make_pair(N, N)) {
      return true;
    }

    for (const auto &[di, dj] : dd) {
      int ii = i + di;
      int jj = j + dj;

      if (in_grid(ii, jj, coords, idx)) {
        pair<int, int> next = {ii, jj};
        int new_cost = cost[current] + 1;
        q.push({new_cost + h(ii, jj), ii, jj});
      }
    }
  }

  return false;
}

int main() {
  ifstream fin("./18.in");
  if (!fin.is_open()) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  vector<pair<int, int>> coords;
  string line;

  while (getline(fin, line)) {
    size_t pos = line.find(',');
    int x = stoi(line.substr(0, pos));
    int y = stoi(line.substr(pos + 1));
    coords.push_back({x, y});
  }
  fin.close();

  // Binary search for the first coordinate that is not doable
  int lo = 0, hi = coords.size() - 1;
  while (hi > lo) {
    int mid = (lo + hi) / 2;
    if (doable(coords, mid)) {
      lo = mid + 1;
    } else {
      hi = mid;
    }
  }

  cout << coords[lo - 1].first << "," << coords[lo - 1].second << endl;
  return 0;
}
