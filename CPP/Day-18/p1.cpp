/* --- Day 18: RAM Run ---
You and The Historians look a lot more pixelated than you remember. You're
inside a computer at the North Pole!

Just as you're about to check out your surroundings, a program runs up to you.
"This region of memory isn't safe! The User misunderstood what a pushdown
automaton is and their algorithm is pushing whole bytes down on top of us! Run!"

The algorithm is fast - it's going to cause a byte to fall into your memory
space once every nanosecond! Fortunately, you're faster, and by quickly scanning
the algorithm, you create a list of which bytes will fall (your puzzle input) in
the order they'll land in your memory space.

Your memory space is a two-dimensional grid with coordinates that range from 0
to 70 both horizontally and vertically. However, for the sake of example,
suppose you're on a smaller grid with coordinates that range from 0 to 6 and the
following list of incoming byte positions:

5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0
Each byte position is given as an X,Y coordinate, where X is the distance from
the left edge of your memory space and Y is the distance from the top edge of
your memory space.

You and The Historians are currently in the top left corner of the memory space
(at 0,0) and need to reach the exit in the bottom right corner (at 70,70 in your
memory space, but at 6,6 in this example). You'll need to simulate the falling
bytes to plan out where it will be safe to run; for now, simulate just the first
few bytes falling into your memory space.

As bytes fall into your memory space, they make that coordinate corrupted.
Corrupted memory coordinates cannot be entered by you or The Historians, so
you'll need to plan your route carefully. You also cannot leave the boundaries
of the memory space; your only hope is to reach the exit.

In the above example, if you were to draw the memory space after the first 12
bytes have fallen (using . for safe and # for corrupted), it would look like
this:

...#...
..#..#.
....#..
...#..#
..#..#.
.#..#..
#.#....
You can take steps up, down, left, or right. After just 12 bytes have corrupted
locations in your memory space, the shortest path from the top left corner to
the exit would take 22 steps. Here (marked with O) is one such path:

OO.#OOO
.O#OO#O
.OOO#OO
...#OO#
..#OO#.
.#.O#..
#.#OOOO
Simulate the first kilobyte (1024 bytes) falling onto your memory space.
Afterward, what is the minimum number of steps needed to reach the exit?

Your puzzle answer was 226.*/

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

// Heuristic for A*
int h(int i, int j) { return abs(N - i) + abs(N - j); }

// Check if a position is within the grid and not in the obstacle set
bool in_grid(
    int i, int j,
    const unordered_set<pair<int, int>, hash<pair<int, int>>> &coords) {
  return 0 <= i && i <= N && 0 <= j && j <= N &&
         coords.find({i, j}) == coords.end();
}

// Custom hash function for pair<int, int>
struct pair_hash {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2> &pair) const {
    return hash<T1>()(pair.first) ^ (hash<T2>()(pair.second) << 1);
  }
};

int main() {
  ifstream fin("./18.in");
  if (!fin.is_open()) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  unordered_set<pair<int, int>, pair_hash> coords;
  string line;
  int count = 0;

  while (getline(fin, line) && count < 1024) {
    size_t pos = line.find(',');
    int x = stoi(line.substr(0, pos));
    int y = stoi(line.substr(pos + 1));
    coords.insert({x, y});
    count++;
  }
  fin.close();

  // Priority queue for A*
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
      cout << cost[current] << endl;
      break;
    }

    for (const auto &[di, dj] : dd) {
      int ii = i + di;
      int jj = j + dj;

      if (in_grid(ii, jj, coords)) {
        pair<int, int> next = {ii, jj};
        int new_cost = cost[current] + 1;
        q.push({new_cost + h(ii, jj), ii, jj});
      }
    }
  }

  return 0;
}
