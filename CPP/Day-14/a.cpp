/* --- Day 14: Restroom Redoubt ---
One of The Historians needs to use the bathroom; fortunately, you know there's a
bathroom near an unvisited location on their list, and so you're all quickly
teleported directly to the lobby of Easter Bunny Headquarters.

Unfortunately, EBHQ seems to have "improved" bathroom security again after your
last visit. The area outside the bathroom is swarming with robots!

To get The Historian safely to the bathroom, you'll need a way to predict where
the robots will be in the future. Fortunately, they all seem to be moving on the
tile floor in predictable straight lines.

You make a list (your puzzle input) of all of the robots' current positions (p)
and velocities (v), one robot per line. For example:

p=0,4 v=3,-3
p=6,3 v=-1,-3
p=10,3 v=-1,2
p=2,0 v=2,-1
p=0,0 v=1,3
p=3,0 v=-2,-2
p=7,6 v=-1,-3
p=3,0 v=-1,-2
p=9,3 v=2,3
p=7,3 v=-1,2
p=2,4 v=2,-3
p=9,5 v=-3,-3
Each robot's position is given as p=x,y where x represents the number of tiles
the robot is from the left wall and y represents the number of tiles from the
top wall (when viewed from above). So, a position of p=0,0 means the robot is
all the way in the top-left corner.

Each robot's velocity is given as v=x,y where x and y are given in tiles per
second. Positive x means the robot is moving to the right, and positive y means
the robot is moving down. So, a velocity of v=1,-2 means that each second, the
robot moves 1 tile to the right and 2 tiles up.

The robots outside the actual bathroom are in a space which is 101 tiles wide
and 103 tiles tall (when viewed from above). However, in this example, the
robots are in a space which is only 11 tiles wide and 7 tiles tall.

The robots are good at navigating over/under each other (due to a combination of
springs, extendable legs, and quadcopters), so they can share the same tile and
don't interact with each other. Visually, the number of robots on each tile in
this example looks like this:

1.12.......
...........
...........
......11.11
1.1........
.........1.
.......1...
These robots have a unique feature for maximum bathroom security: they can
teleport. When a robot would run into an edge of the space they're in, they
instead teleport to the other side, effectively wrapping around the edges. Here
is what robot p=2,4 v=2,-3 does for the first few seconds:

Initial state:
...........
...........
...........
...........
..1........
...........
...........

After 1 second:
...........
....1......
...........
...........
...........
...........
...........

After 2 seconds:
...........
...........
...........
...........
...........
......1....
...........

After 3 seconds:
...........
...........
........1..
...........
...........
...........
...........

After 4 seconds:
...........
...........
...........
...........
...........
...........
..........1

After 5 seconds:
...........
...........
...........
.1.........
...........
...........
...........
The Historian can't wait much longer, so you don't have to simulate the robots
for very long. Where will the robots be after 100 seconds?

In the above example, the number of robots on each tile after 100 seconds has
elapsed looks like this:

......2..1.
...........
1..........
.11........
.....1.....
...12......
.1....1....
To determine the safest area, count the number of robots in each quadrant after
100 seconds. Robots that are exactly in the middle (horizontally or vertically)
don't count as being in any quadrant, so the only relevant robots are:

..... 2..1.
..... .....
1.... .....

..... .....
...12 .....
.1... 1....
In this example, the quadrants contain 1, 3, 4, and 1 robot. Multiplying these
together gives a total safety factor of 12.

Predict the motion of the robots in your list within a space which is 101 tiles
wide and 103 tiles tall. What will the safety factor be after exactly 100
seconds have elapsed?

*/

#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const vector<pair<int, int>> DIRS = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // up, right, down, left

// Utility function to extract integers from a string
vector<int> ints(const string &s) {
  vector<int> result;
  regex re("-?\\d+");
  auto words_begin = sregex_iterator(s.begin(), s.end(), re);
  auto words_end = sregex_iterator();
  for (auto it = words_begin; it != words_end; ++it) {
    result.push_back(stoi(it->str()));
  }
  return result;
}

int main(int argc, char *argv[]) {
  // Read input file
  string infile = (argc >= 2) ? argv[1] : "14.in";
  ifstream input(infile);
  if (!input.is_open()) {
    cerr << "Error: Cannot open file " << infile << endl;
    return 1;
  }
  stringstream buffer;
  buffer << input.rdbuf();
  string D = buffer.str();

  // Variables and constants
  int X = 101, Y = 103;
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
  vector<tuple<int, int, int, int>> robots;

  // Parse input
  string line;
  stringstream ss(D);
  while (getline(ss, line)) {
    auto coords = ints(line);
    robots.emplace_back(coords[0], coords[1], coords[2], coords[3]);
  }

  for (int t = 1; t < 1000000; ++t) {
    vector<vector<char>> G(Y, vector<char>(X, '.'));
    if (t == 100) {
      q1 = q2 = q3 = q4 = 0;
      int mx = X / 2, my = Y / 2;

      // Process robots at t = 100
      for (auto &[px, py, vx, vy] : robots) {
        px += vx;
        py += vy;
        px %= X;
        py %= Y;
        if (px < 0)
          px += X;
        if (py < 0)
          py += Y;

        assert(0 <= px && px < X);
        assert(0 <= py && py < Y);

        if (px < mx && py < my)
          ++q1;
        if (px > mx && py < my)
          ++q2;
        if (px < mx && py > my)
          ++q3;
        if (px > mx && py > my)
          ++q4;

        G[py][px] = '#';
      }

      cout << "Part 1: " << (q1 * q2 * q3 * q4) << endl;
    } else {
      for (auto &[px, py, vx, vy] : robots) {
        px += vx;
        py += vy;
        px %= X;
        py %= Y;
        if (px < 0)
          px += X;
        if (py < 0)
          py += Y;

        assert(0 <= px && px < X);
        assert(0 <= py && py < Y);
        G[py][px] = '#';
      }
    }

    // Count connected components
    int components = 0;
    set<pair<int, int>> SEEN;
    for (int y = 0; y < Y; ++y) {
      for (int x = 0; x < X; ++x) {
        if (G[y][x] == '#' && !SEEN.count({x, y})) {
          ++components;
          deque<pair<int, int>> Q = {{x, y}};
          while (!Q.empty()) {
            auto [x2, y2] = Q.front();
            Q.pop_front();
            if (SEEN.count({x2, y2}))
              continue;
            SEEN.insert({x2, y2});
            for (auto [dx, dy] : DIRS) {
              int xx = x2 + dx, yy = y2 + dy;
              if (0 <= xx && xx < X && 0 <= yy && yy < Y && G[yy][xx] == '#') {
                Q.emplace_back(xx, yy);
              }
            }
          }
        }
      }
    }

    if (components <= 200) {
      cout << "Time: " << t << endl;
      for (const auto &row : G) {
        for (char c : row)
          cout << c;
        cout << endl;
      }
      break;
    }
  }

  return 0;
}
