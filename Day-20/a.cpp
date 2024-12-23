/* The Historians are quite pixelated again. This time, a massive, black
building looms over you - you're right outside the CPU!

While The Historians get to work, a nearby program sees that you're idle and
challenges you to a race. Apparently, you've arrived just in time for the
frequently-held race condition festival!

The race takes place on a particularly long and twisting code path; programs
compete to see who can finish in the fewest picoseconds. The winner even gets
their very own mutex!

They hand you a map of the racetrack (your puzzle input). For example:

###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############
The map consists of track (.) - including the start (S) and end (E) positions
(both of which also count as track) - and walls (#).

When a program runs through the racetrack, it starts at the start position.
Then, it is allowed to move up, down, left, or right; each such move takes 1
picosecond. The goal is to reach the end position as quickly as possible. In
this example racetrack, the fastest time is 84 picoseconds.

Because there is only a single path from the start to the end and the programs
all go the same speed, the races used to be pretty boring. To make things more
interesting, they introduced a new rule to the races: programs are allowed to
cheat.

The rules for cheating are very strict. Exactly once during a race, a program
may disable collision for up to 2 picoseconds. This allows the program to pass
through walls as if they were regular track. At the end of the cheat, the
program must be back on normal track again; otherwise, it will receive a
segmentation fault and get disqualified.

So, a program could complete the course in 72 picoseconds (saving 12
picoseconds) by cheating for the two moves marked 1 and 2:

###############
#...#...12....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############
Or, a program could complete the course in 64 picoseconds (saving 20
picoseconds) by cheating for the two moves marked 1 and 2:

###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...12..#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############
This cheat saves 38 picoseconds:

###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..E#...#...#
###.####1##.###
#...###.2.#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############
This cheat saves 64 picoseconds and takes the program directly to the end:

###############
#...#...#.....#
#.#.#.#.#.###.#
#S#...#.#.#...#
#######.#.#.###
#######.#.#...#
#######.#.###.#
###..21...#...#
###.#######.###
#...###...#...#
#.#####.#.###.#
#.#...#.#.#...#
#.#.#.#.#.#.###
#...#...#...###
###############
Each cheat has a distinct start position (the position where the cheat is
activated, just before the first move that is allowed to go through walls) and
end position; cheats are uniquely identified by their start position and end
position.

In this example, the total number of cheats (grouped by the amount of time they
save) are as follows:

There are 14 cheats that save 2 picoseconds.
There are 14 cheats that save 4 picoseconds.
There are 2 cheats that save 6 picoseconds.
There are 4 cheats that save 8 picoseconds.
There are 2 cheats that save 10 picoseconds.
There are 3 cheats that save 12 picoseconds.
There is one cheat that saves 20 picoseconds.
There is one cheat that saves 36 picoseconds.
There is one cheat that saves 38 picoseconds.
There is one cheat that saves 40 picoseconds.
There is one cheat that saves 64 picoseconds.
You aren't sure what the conditions of the racetrack will be like, so to give
yourself as many options as possible, you'll need a list of the best cheats. How
many cheats would save you at least 100 picoseconds?

*/

#include <climits>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};
const char WALL = '#';
const char TRACK = '.';
const char START = 'S';
const char END = 'E';

// A structure to store a position in the grid
struct Point {
  int x, y;
};

// Function to check if a point is within the grid bounds
bool isInBounds(int x, int y, int rows, int cols) {
  return x >= 0 && x < rows && y >= 0 && y < cols;
}

// BFS function to find the shortest time from start to end
int bfsWithoutCheat(vector<string> &grid, Point start, Point end, int rows,
                    int cols) {
  vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
  queue<Point> q;
  dist[start.x][start.y] = 0;
  q.push(start);

  while (!q.empty()) {
    Point p = q.front();
    q.pop();
    for (int i = 0; i < 4; ++i) {
      int nx = p.x + dx[i], ny = p.y + dy[i];
      if (isInBounds(nx, ny, rows, cols) && grid[nx][ny] != WALL &&
          dist[nx][ny] == INT_MAX) {
        dist[nx][ny] = dist[p.x][p.y] + 1;
        q.push({nx, ny});
      }
    }
  }
  return dist[end.x][end.y];
}

// BFS function to calculate the shortest time with a cheat at a particular
// position
int bfsWithCheat(vector<string> &grid, Point start, Point end, Point cheatStart,
                 Point cheatEnd, int rows, int cols) {
  vector<vector<int>> dist(rows, vector<int>(cols, INT_MAX));
  queue<Point> q;
  dist[start.x][start.y] = 0;
  q.push(start);

  bool cheatUsed = false;

  while (!q.empty()) {
    Point p = q.front();
    q.pop();
    for (int i = 0; i < 4; ++i) {
      int nx = p.x + dx[i], ny = p.y + dy[i];
      if (isInBounds(nx, ny, rows, cols)) {
        // If it's a wall, we can use the cheat to pass through it
        if (grid[nx][ny] == WALL && !cheatUsed) {
          if (nx == cheatEnd.x && ny == cheatEnd.y) {
            dist[nx][ny] = dist[p.x][p.y] + 1;
            q.push({nx, ny});
            cheatUsed = true;
          }
        } else if (grid[nx][ny] != WALL && dist[nx][ny] == INT_MAX) {
          dist[nx][ny] = dist[p.x][p.y] + 1;
          q.push({nx, ny});
        }
      }
    }
  }
  return dist[end.x][end.y];
}

int main() {
  // Read the input grid (racetrack)
  vector<string> grid;
  string line;
  while (getline(cin, line)) {
    grid.push_back(line);
  }

  int rows = grid.size();
  int cols = grid[0].size();
  Point start, end;

  // Find the positions of 'S' (start) and 'E' (end)
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (grid[i][j] == START) {
        start = {i, j};
      } else if (grid[i][j] == END) {
        end = {i, j};
      }
    }
  }

  // Calculate the time from start to end without cheating
  int normalTime = bfsWithoutCheat(grid, start, end, rows, cols);

  // Now, let's find the cheats and track their time savings
  vector<int> timeSavings;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (grid[i][j] != WALL) {
        // Try using the cheat at this position
        for (int x = 0; x < rows; ++x) {
          for (int y = 0; y < cols; ++y) {
            if (grid[x][y] != WALL && (x != i || y != j)) {
              int timeWithCheat =
                  bfsWithCheat(grid, start, end, {i, j}, {x, y}, rows, cols);
              int timeSaved = normalTime - timeWithCheat;
              if (timeSaved >= 100) {
                timeSavings.push_back(timeSaved);
              }
            }
          }
        }
      }
    }
  }

  // Output the number of cheats that save at least 100 picoseconds
  cout << timeSavings.size() << endl;

  return 0;
}
