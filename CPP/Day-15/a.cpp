/* --- Day 15: Warehouse Woes ---
You appear back inside your own mini submarine! Each Historian drives their mini
submarine in a different direction; maybe the Chief has his own submarine down
here somewhere as well?

You look up to see a vast school of lanternfish swimming past you. On closer
inspection, they seem quite anxious, so you drive your mini submarine over to
see if you can help.

Because lanternfish populations grow rapidly, they need a lot of food, and that
food needs to be stored somewhere. That's why these lanternfish have built
elaborate warehouse complexes operated by robots!

These lanternfish seem so anxious because they have lost control of the robot
that operates one of their most important warehouses! It is currently running
amok, pushing around boxes in the warehouse with no regard for lanternfish
logistics or lanternfish inventory management strategies.

Right now, none of the lanternfish are brave enough to swim up to an
unpredictable robot so they could shut it off. However, if you could anticipate
the robot's movements, maybe they could find a safe option.

The lanternfish already have a map of the warehouse and a list of movements the
robot will attempt to make (your puzzle input). The problem is that the
movements will sometimes fail as boxes are shifted around, making the actual
movements of the robot difficult to predict.

For example:

##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########

<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^
As the robot (@) attempts to move, if there are any boxes (O) in the way, the
robot will also attempt to push those boxes. However, if this action would cause
the robot or a box to move into a wall (#), nothing moves instead, including the
robot. The initial positions of these are shown on the map at the top of the
document the lanternfish gave you.

The rest of the document describes the moves (^ for up, v for down, < for left,
> for right) that the robot will attempt to make, in order. (The moves form a
single giant sequence; they are broken into multiple lines just to make
copy-pasting easier. Newlines within the move sequence should be ignored.)

Here is a smaller example to get started:

########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

<^^>>>vv<v>>v<<
Were the robot to attempt the given sequence of moves, it would push around the
boxes as follows:

Initial state:
########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move <:
########
#..O.O.#
##@.O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move ^:
########
#.@O.O.#
##..O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move ^:
########
#.@O.O.#
##..O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move >:
########
#..@OO.#
##..O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move >:
########
#...@OO#
##..O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move >:
########
#...@OO#
##..O..#
#...O..#
#.#.O..#
#...O..#
#......#
########

Move v:
########
#....OO#
##..@..#
#...O..#
#.#.O..#
#...O..#
#...O..#
########

Move v:
########
#....OO#
##..@..#
#...O..#
#.#.O..#
#...O..#
#...O..#
########

Move <:
########
#....OO#
##.@...#
#...O..#
#.#.O..#
#...O..#
#...O..#
########

Move v:
########
#....OO#
##.....#
#..@O..#
#.#.O..#
#...O..#
#...O..#
########

Move >:
########
#....OO#
##.....#
#...@O.#
#.#.O..#
#...O..#
#...O..#
########

Move >:
########
#....OO#
##.....#
#....@O#
#.#.O..#
#...O..#
#...O..#
########

Move v:
########
#....OO#
##.....#
#.....O#
#.#.O@.#
#...O..#
#...O..#
########

Move <:
########
#....OO#
##.....#
#.....O#
#.#O@..#
#...O..#
#...O..#
########

Move <:
########
#....OO#
##.....#
#.....O#
#.#O@..#
#...O..#
#...O..#
########
The larger example has many more moves; after the robot has finished those
moves, the warehouse would look like this:

##########
#.O.O.OOO#
#........#
#OO......#
#OO@.....#
#O#.....O#
#O.....OO#
#O.....OO#
#OO....OO#
##########
The lanternfish use their own custom Goods Positioning System (GPS for short) to
track the locations of the boxes. The GPS coordinate of a box is equal to 100
times its distance from the top edge of the map plus its distance from the left
edge of the map. (This process does not stop at wall tiles; measure all the way
to the edges of the map.)

So, the box shown below has a distance of 1 from the top edge of the map and 4
from the left edge of the map, resulting in a GPS coordinate of 100 * 1 + 4 =
104.

#######
#...O..
#......
The lanternfish would like to know the sum of all boxes' GPS coordinates after
the robot finishes moving. In the larger example, the sum of all boxes' GPS
coordinates is 10092. In the smaller example, the sum is 2028.

Predict the motion of the robot and boxes in the warehouse. After the robot is
finished moving, what is the sum of all boxes' GPS coordinates?

Your puzzle answer was 1505963.

--- Part Two ---
The lanternfish use your information to find a safe moment to swim in and turn
off the malfunctioning robot! Just as they start preparing a festival in your
honor, reports start coming in that a second warehouse's robot is also
malfunctioning.

This warehouse's layout is surprisingly similar to the one you just helped.
There is one key difference: everything except the robot is twice as wide! The
robot's list of movements doesn't change.

To get the wider warehouse's map, start with your original map and, for each
tile, make the following changes:

If the tile is #, the new map contains ## instead.
If the tile is O, the new map contains [] instead.
If the tile is ., the new map contains .. instead.
If the tile is @, the new map contains @. instead.
This will produce a new warehouse map which is twice as wide and with wide boxes
that are represented by []. (The robot does not change size.)

The larger example from before would now look like this:

####################
##....[]....[]..[]##
##............[]..##
##..[][]....[]..[]##
##....[]@.....[]..##
##[]##....[]......##
##[]....[]....[]..##
##..[][]..[]..[][]##
##........[]......##
####################
Because boxes are now twice as wide but the robot is still the same size and
speed, boxes can be aligned such that they directly push two other boxes at
once. For example, consider this situation:

#######
#...#.#
#.....#
#..OO@#
#..O..#
#.....#
#######

<vv<<^^<<^^
After appropriately resizing this map, the robot would push around these boxes
as follows:

Initial state:
##############
##......##..##
##..........##
##....[][]@.##
##....[]....##
##..........##
##############

Move <:
##############
##......##..##
##..........##
##...[][]@..##
##....[]....##
##..........##
##############

Move v:
##############
##......##..##
##..........##
##...[][]...##
##....[].@..##
##..........##
##############

Move v:
##############
##......##..##
##..........##
##...[][]...##
##....[]....##
##.......@..##
##############

Move <:
##############
##......##..##
##..........##
##...[][]...##
##....[]....##
##......@...##
##############

Move <:
##############
##......##..##
##..........##
##...[][]...##
##....[]....##
##.....@....##
##############

Move ^:
##############
##......##..##
##...[][]...##
##....[]....##
##.....@....##
##..........##
##############

Move ^:
##############
##......##..##
##...[][]...##
##....[]....##
##.....@....##
##..........##
##############

Move <:
##############
##......##..##
##...[][]...##
##....[]....##
##....@.....##
##..........##
##############

Move <:
##############
##......##..##
##...[][]...##
##....[]....##
##...@......##
##..........##
##############

Move ^:
##############
##......##..##
##...[][]...##
##...@[]....##
##..........##
##..........##
##############

Move ^:
##############
##...[].##..##
##...@.[]...##
##....[]....##
##..........##
##..........##
##############
This warehouse also uses GPS to locate the boxes. For these larger boxes,
distances are measured from the edge of the map to the closest edge of the box
in question. So, the box shown below has a distance of 1 from the top edge of
the map and 5 from the left edge of the map, resulting in a GPS coordinate of
100 * 1 + 5 = 105.

##########
##...[]...
##........
In the scaled-up version of the larger example from above, after the robot has
finished all of its moves, the warehouse would look like this:

####################
##[].......[].[][]##
##[]...........[].##
##[]........[][][]##
##[]......[]....[]##
##..##......[]....##
##..[]............##
##..@......[].[][]##
##......[][]..[]..##
####################
The sum of these boxes' GPS coordinates is 9021.

Predict the motion of the robot and boxes in this new, scaled-up warehouse. What
is the sum of all boxes' final GPS coordinates?

Your puzzle answer was 1543141.
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// Utility functions
std::vector<std::string> split(const std::string &s,
                               const std::string &delimiter) {
  std::vector<std::string> tokens;
  size_t start = 0, end = 0;
  while ((end = s.find(delimiter, start)) != std::string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + delimiter.length();
  }
  if (start < s.length()) {
    tokens.push_back(s.substr(start));
  }
  return tokens;
}

std::vector<int> ints(const std::string &s) {
  std::vector<int> result;
  std::regex number_regex("-?\\d+");
  std::sregex_iterator it(s.begin(), s.end(), number_regex);
  std::sregex_iterator end;

  while (it != end) {
    result.push_back(std::stoi(it->str()));
    ++it;
  }
  return result;
}

void pr(long long s) {
  std::cout << s << std::endl;
  // Note: No direct equivalent to pyperclip in standard C++
}

int solve(std::vector<std::string> &G, std::string &instrs, bool part2) {
  int R = G.size();
  int C = G[0].length();

  // Create 2D vector from grid
  std::vector<std::vector<char>> grid(R, std::vector<char>(C));
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      grid[r][c] = G[r][c];
    }
  }

  if (part2) {
    std::vector<std::vector<char>> big_grid;
    for (int r = 0; r < R; ++r) {
      std::vector<char> row;
      for (int c = 0; c < C; ++c) {
        if (grid[r][c] == '#') {
          row.push_back('#');
          row.push_back('#');
        }
        if (grid[r][c] == 'O') {
          row.push_back('[');
          row.push_back(']');
        }
        if (grid[r][c] == '.') {
          row.push_back('.');
          row.push_back('.');
        }
        if (grid[r][c] == '@') {
          row.push_back('@');
          row.push_back('.');
        }
      }
      big_grid.push_back(row);
    }
    grid = big_grid;
    C *= 2;
  }

  // Find start position
  int sr = -1, sc = -1;
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (grid[r][c] == '@') {
        sr = r;
        sc = c;
        grid[r][c] = '.';
        break;
      }
    }
    if (sr != -1)
      break;
  }

  int r = sr, c = sc;

  // Directions mapping
  std::map<char, std::pair<int, int>> dirs = {
      {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};

  // Process instructions
  for (char inst : instrs) {
    if (inst == '\n')
      continue;

    auto [dr, dc] = dirs[inst];
    int rr = r + dr, cc = c + dc;

    if (grid[rr][cc] == '#')
      continue;
    else if (grid[rr][cc] == '.') {
      r = rr;
      c = cc;
    } else if (grid[rr][cc] == '[' || grid[rr][cc] == ']' ||
               grid[rr][cc] == 'O') {
      std::queue<std::pair<int, int>> Q;
      std::set<std::pair<int, int>> SEEN;
      Q.push({r, c});
      bool ok = true;

      while (!Q.empty()) {
        auto [curr_r, curr_c] = Q.front();
        Q.pop();

        if (SEEN.count({curr_r, curr_c}))
          continue;
        SEEN.insert({curr_r, curr_c});

        int next_r = curr_r + dr, next_c = curr_c + dc;

        if (grid[next_r][next_c] == '#') {
          ok = false;
          break;
        }

        if (grid[next_r][next_c] == 'O')
          Q.push({next_r, next_c});
        if (grid[next_r][next_c] == '[') {
          Q.push({next_r, next_c});
          Q.push({next_r, next_c + 1});
        }
        if (grid[next_r][next_c] == ']') {
          Q.push({next_r, next_c});
          Q.push({next_r, next_c - 1});
        }
      }

      if (!ok)
        continue;

      while (!SEEN.empty()) {
        std::set<std::pair<int, int>> new_seen;
        for (const auto &[curr_r, curr_c] : SEEN) {
          int next_r = curr_r + dr, next_c = curr_c + dc;
          if (!SEEN.count({next_r, next_c})) {
            grid[next_r][next_c] = grid[curr_r][curr_c];
            grid[curr_r][curr_c] = '.';
            new_seen.insert({curr_r, curr_c});
          }
        }
        for (const auto &pos : new_seen) {
          SEEN.erase(pos);
        }
      }

      r += dr;
      c += dc;
    }
  }

  // Calculate answer
  int ans = 0;
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (grid[r][c] == '[' || grid[r][c] == 'O') {
        ans += 100 * r + c;
      }
    }
  }

  return ans;
}

int main(int argc, char *argv[]) {
  std::string infile = (argc >= 2) ? argv[1] : "15.in";
  std::ifstream file(infile);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string D = buffer.str();

  // Split input
  auto parts = split(D, "\n\n");
  std::vector<std::string> G = split(parts[0], "\n");
  std::string instrs = parts[1];

  pr(solve(G, instrs, false));
  pr(solve(G, instrs, true));

  return 0;
}
