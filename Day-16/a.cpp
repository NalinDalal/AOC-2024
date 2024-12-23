/*It's time again for the Reindeer Olympics! This year, the big event is the
Reindeer Maze, where the Reindeer compete for the lowest score.

You and The Historians arrive to search for the Chief right as the event is
about to start. It wouldn't hurt to watch a little, right?

The Reindeer start on the Start Tile (marked S) facing East and need to reach
the End Tile (marked E). They can move forward one tile at a time (increasing
their score by 1 point), but never into a wall (#). They can also rotate
clockwise or counterclockwise 90 degrees at a time (increasing their score by
1000 points).

To figure out the best place to sit, you start by grabbing a map (your puzzle
input) from a nearby kiosk. For example:

###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############
There are many paths through this maze, but taking any of the best paths would
incur a score of only 7036. This can be achieved by taking a total of 36 steps
forward and turning 90 degrees a total of 7 times:


###############
#.......#....E#
#.#.###.#.###^#
#.....#.#...#^#
#.###.#####.#^#
#.#.#.......#^#
#.#.#####.###^#
#..>>>>>>>>v#^#
###^#.#####v#^#
#>>^#.....#v#^#
#^#.#.###.#v#^#
#^....#...#v#^#
#^###.#.#.#v#^#
#S..#.....#>>^#
###############
Here's a second example:

#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################
In this maze, the best paths cost 11048 points; following one such path would
look like this:

#################
#...#...#...#..E#
#.#.#.#.#.#.#.#^#
#.#.#.#...#...#^#
#.#.#.#.###.#.#^#
#>>v#.#.#.....#^#
#^#v#.#.#.#####^#
#^#v..#.#.#>>>>^#
#^#v#####.#^###.#
#^#v#..>>>>^#...#
#^#v###^#####.###
#^#v#>>^#.....#.#
#^#v#^#####.###.#
#^#v#^........#.#
#^#v#^#########.#
#S#>>^..........#
#################
Note that the path shown above includes one 90 degree turn as the very first
move, rotating the Reindeer from facing East to facing North.

Analyze your map carefully. What is the lowest score a Reindeer could possibly
get?

*/

#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef tuple<int, int, int> State;           // (row, col, direction)
typedef tuple<int, int, int, int> QueueState; // (distance, row, col, direction)

// Parse the input file
void parseInput(const string &filePath, vector<string> &grid, int &startRow,
                int &startCol, int &endRow, int &endCol) {
  ifstream file(filePath);
  string line;
  startRow = startCol = endRow = endCol = -1;

  while (getline(file, line)) {
    grid.push_back(line);
  }

  for (int r = 0; r < grid.size(); ++r) {
    for (int c = 0; c < grid[0].size(); ++c) {
      if (grid[r][c] == 'S') {
        startRow = r;
        startCol = c;
      } else if (grid[r][c] == 'E') {
        endRow = r;
        endCol = c;
      }
    }
  }
}

// Dijkstra's algorithm
unordered_map<State, int> dijkstra(const vector<string> &grid, int startRow,
                                   int startCol,
                                   const vector<int> &initialDirections,
                                   bool forward) {
  int rowCount = grid.size();
  int colCount = grid[0].size();

  // Direction vectors: Up, Right, Down, Left
  vector<pair<int, int>> directions = {
      {-1, 0}, // Up
      {0, 1},  // Right
      {1, 0},  // Down
      {0, -1}  // Left
  };
  priority_queue<QueueState, vector<QueueState>, greater<>> heap;

  unordered_set<State> visited;
  unordered_map<State, int> distances;

  // Initialize the heap
  for (int dir : initialDirections) {
    heap.emplace(0, startRow, startCol, dir);
  }

  // Process the priority queue
  while (!heap.empty()) {
    auto [distance, row, col, direction] = heap.top();
    heap.pop();

    State state = {row, col, direction};

    // Record the distance if not already recorded
    if (distances.find(state) == distances.end()) {
      distances[state] = distance;
    }

    // Skip if already visited
    if (visited.count(state)) {
      continue;
    }
    visited.insert(state);

    // Move in the current direction
    int dr = forward ? directions[direction].first
                     : directions[(direction + 2) % 4].first;
    int dc = forward ? directions[direction].second
                     : directions[(direction + 2) % 4].second;
    int nextRow = row + dr;
    int nextCol = col + dc;

    if (nextRow >= 0 && nextRow < rowCount && nextCol >= 0 &&
        nextCol < colCount && grid[nextRow][nextCol] != '#') {
      heap.emplace(distance + 1, nextRow, nextCol, direction);
    }

    // Turn left or right
    heap.emplace(distance + 1000, row, col, (direction + 1) % 4); // Turn right
    heap.emplace(distance + 1000, row, col, (direction + 3) % 4); // Turn left
  }

  return distances;
}

int main() {
  string filePath = "../data/input";
  vector<string> grid;
  int startRow, startCol, endRow, endCol;

  parseInput(filePath, grid, startRow, startCol, endRow, endCol);

  // Part 1: Shortest path from start to end
  auto distances = dijkstra(grid, startRow, startCol, {0, 1, 2, 3}, true);

  int bestDistance = INT_MAX;
  for (int dir = 0; dir < 4; ++dir) {
    State endState = {endRow, endCol, dir};
    if (distances.find(endState) != distances.end()) {
      bestDistance = min(bestDistance, distances[endState]);
    }
  }

  cout << "Part 1: " << bestDistance << endl;

  // Part 2: Reverse Dijkstra and count optimal paths
  auto distances2 = dijkstra(grid, endRow, endCol, {0, 1, 2, 3}, false);

  set<pair<int, int>> optimalPaths;
  for (int r = 0; r < grid.size(); ++r) {
    for (int c = 0; c < grid[0].size(); ++c) {
      for (int dir = 0; dir < 4; ++dir) {
        State state = {r, c, dir};
        if (distances.find(state) != distances.end() &&
            distances2.find(state) != distances2.end() &&
            distances[state] + distances2[state] == bestDistance) {
          optimalPaths.emplace(r, c);
        }
      }
    }
  }

  cout << "Part 2: " << optimalPaths.size() << endl;

  return 0;
}
