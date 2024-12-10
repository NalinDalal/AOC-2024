#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using Grid = vector<vector<int>>;

// Function to parse the input file into a 2D grid of integers
Grid parseInput(const string &filename) {
  ifstream infile(filename);
  string line;
  Grid grid;
  while (getline(infile, line)) {
    vector<int> row;
    for (char ch : line) {
      row.push_back(ch - '0'); // Convert char to int
    }
    grid.push_back(row);
  }
  return grid;
}

// Helper function for "ways1"
int ways1(const Grid &G, int sr, int sc) {
  int R = G.size(), C = G[0].size();
  deque<pair<int, int>> Q = {{sr, sc}};
  int ans = 0;
  set<pair<int, int>> SEEN;

  while (!Q.empty()) {
    auto [r, c] = Q.front();
    Q.pop_front();
    if (SEEN.count({r, c}))
      continue;
    SEEN.insert({r, c});

    if (G[r][c] == 0)
      ans++;
    for (auto [dr, dc] :
         vector<pair<int, int>>{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}) {
      int rr = r + dr, cc = c + dc;
      if (rr >= 0 && rr < R && cc >= 0 && cc < C && G[rr][cc] == G[r][c] - 1) {
        Q.emplace_back(rr, cc);
      }
    }
  }

  return ans;
}

// Memoized recursive function for "ways"
int ways(const Grid &G, int r, int c, map<pair<int, int>, int> &DP) {
  if (G[r][c] == 0)
    return 1;
  if (DP.count({r, c}))
    return DP[{r, c}];

  int R = G.size(), C = G[0].size();
  int ans = 0;

  for (auto [dr, dc] :
       vector<pair<int, int>>{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}) {
    int rr = r + dr, cc = c + dc;
    if (rr >= 0 && rr < R && cc >= 0 && cc < C && G[rr][cc] == G[r][c] - 1) {
      ans += ways(G, rr, cc, DP);
    }
  }

  DP[{r, c}] = ans;
  return ans;
}

int main(int argc, char *argv[]) {
  string infile = (argc >= 2) ? argv[1] : "10.in";

  // Parse input
  Grid G = parseInput(infile);
  int R = G.size(), C = G[0].size();
  int p1 = 0, p2 = 0;

  map<pair<int, int>, int> DP;

  // Main loop
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (G[r][c] == 9) {
        p1 += ways1(G, r, c);
        p2 += ways(G, r, c, DP);
      }
    }
  }

  // Print results
  cout << p1 << endl;
  cout << p2 << endl;

  return 0;
}
