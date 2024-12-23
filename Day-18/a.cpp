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

*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

const int N = 71;
const std::vector<std::pair<int, int>> DIRS = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // up right down left

// Helper function to split string
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

// Helper function to extract integers from string
std::vector<int> ints(const std::string &s) {
  std::vector<int> result;
  std::regex int_regex("-?\\d+");
  auto words_begin = std::sregex_iterator(s.begin(), s.end(), int_regex);
  auto words_end = std::sregex_iterator();

  for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    std::smatch match = *i;
    result.push_back(std::stoi(match.str()));
  }
  return result;
}

int main(int argc, char *argv[]) {
  // Determine input file
  std::string infile = (argc >= 2) ? argv[1] : "18.in";

  // Read input file
  std::ifstream file(infile);
  std::string D;
  std::string line;
  std::string content;
  while (std::getline(file, line)) {
    content += line + "\n";
  }
  D = content;

  // Remove trailing newline
  if (!D.empty() && D.back() == '\n') {
    D.pop_back();
  }

  // Create grid
  std::vector<std::vector<char>> G(N, std::vector<char>(N, '.'));

  // Process lines
  std::istringstream iss(D);
  std::string line_input;
  int ans = 0;

  while (std::getline(iss, line_input)) {
    std::vector<int> coords = ints(line_input);
    if (coords.size() >= 2) {
      int x = coords[0], y = coords[1];
      if (0 <= y && y < N && 0 <= x && x < N) {
        G[y][x] = '#';
      }

      // BFS
      std::queue<std::tuple<int, int, int>> Q;
      Q.push({0, 0, 0}); // distance, row, col
      std::set<std::pair<int, int>> SEEN;
      bool ok = false;

      while (!Q.empty()) {
        auto [d, r, c] = Q.front();
        Q.pop();

        if (r == N - 1 && c == N - 1) {
          if (line_input.find("1023") != std::string::npos) {
            std::cout << d << std::endl;
          }
          ok = true;
          break;
        }

        if (SEEN.count({r, c}))
          continue;
        SEEN.insert({r, c});

        for (const auto &[dr, dc] : DIRS) {
          int rr = r + dr;
          int cc = c + dc;

          if (0 <= rr && rr < N && 0 <= cc && cc < N && G[rr][cc] != '#') {
            Q.push({d + 1, rr, cc});
          }
        }
      }

      if (!ok) {
        std::cout << x << "," << y << std::endl;
        break;
      }
    }
  }

  return 0;
}
