/* --- Day 6: Guard Gallivant ---
The Historians use their fancy device again, this time to whisk you all away to
the North Pole prototype suit manufacturing lab... in the year 1518! It turns
out that having direct access to history is very convenient for a group of
historians.

You still have to be careful of time paradoxes, and so it will be important to
avoid anyone from 1518 while The Historians search for the Chief. Unfortunately,
a single guard is patrolling this part of the lab.

Maybe you can work out where the guard will go ahead of time so that The
Historians can search safely?

You start by making a map (your puzzle input) of the situation. For example:

....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...
The map shows the current position of the guard with ^ (to indicate the guard is
currently facing up from the perspective of the map). Any obstructions - crates,
desks, alchemical reactors, etc. - are shown as #.

Lab guards in 1518 follow a very strict patrol protocol which involves
repeatedly following these steps:

If there is something directly in front of you, turn right 90 degrees.
Otherwise, take a step forward.
Following the above protocol, the guard moves up several times until she reaches
an obstacle (in this case, a pile of failed suit prototypes):

....#.....
....^....#
..........
..#.......
.......#..
..........
.#........
........#.
#.........
......#...
Because there is now an obstacle in front of the guard, she turns right before
continuing straight in her new facing direction:

....#.....
........>#
..........
..#.......
.......#..
..........
.#........
........#.
#.........
......#...
Reaching another obstacle (a spool of several very long polymers), she turns
right again and continues downward:

....#.....
.........#
..........
..#.......
.......#..
..........
.#......v.
........#.
#.........
......#...
This process continues for a while, but the guard eventually leaves the mapped
area (after walking past a tank of universal solvent):

....#.....
.........#
..........
..#.......
.......#..
..........
.#........
........#.
#.........
......#v..
By predicting the guard's route, you can determine which specific positions in
the lab will be in the patrol path. Including the guard's starting position, the
positions visited by the guard before leaving the area are marked with an X:

....#.....
....XXXXX#
....X...X.
..#.X...X.
..XXXXX#X.
..X.X.X.X.
.#XXXXXXX.
.XXXXXXX#.
#XXXXXXX..
......#X..
In this example, the guard will visit 41 distinct positions on your map.

Predict the path of the guard. How many distinct positions will the guard visit
before leaving the mapped area?

*/
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

int main() {
  string infile = "p1.txt";

  // Read the input file
  ifstream file(infile);
  if (!file) {
    cerr << "Error reading file!" << endl;
    return 1;
  }

  string line;
  vector<string> G;
  while (getline(file, line)) {
    G.push_back(line);
  }

  int R = G.size();
  int C = G[0].size();

  // Locate the guard's starting position
  int sr = 0, sc = 0;
  int direction = 0; // 0=up, 1=right, 2=down, 3=left
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (G[r][c] == '^') {
        sr = r;
        sc = c;
        direction = 0; // Facing up
      }
    }
  }

  // Directional vectors for movement: up, right, down, left
  vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  set<pair<int, int>> visited; // To track distinct positions
  visited.insert({sr, sc});    // Include the starting position

  int r = sr, c = sc;

  while (true) {
    int dr = directions[direction].first;
    int dc = directions[direction].second;

    int rr = r + dr;
    int cc = c + dc;

    // Check if out of bounds
    if (rr < 0 || rr >= R || cc < 0 || cc >= C) {
      break;
    }

    // Check if there's an obstacle
    if (G[rr][cc] == '#') {
      direction = (direction + 1) % 4; // Turn right
    } else {
      // Move forward
      r = rr;
      c = cc;
      visited.insert({r, c});
    }
  }

  // Output the result
  cout << "result:\"" << visited.size() << "\"" << endl;

  return 0;
}
