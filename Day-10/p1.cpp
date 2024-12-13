#include <deque>
#include <fstream> // Include this for ifstream
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

#define pr(x)                                                                  \
  {                                                                            \
    cout << x << endl;                                                         \
  }

int R, C;
vector<vector<int>> G;
unordered_map<string, int> DP; // Memoization for the ways function

// Directions for moving (up, right, down, left)
vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Helper function for counting how many 0s can be reached starting from (sr,
// sc)
int ways1(int sr, int sc) {
  deque<pair<int, int>> Q;
  Q.push_back({sr, sc});
  int ans = 0;
  set<pair<int, int>> SEEN;

  while (!Q.empty()) {
    auto [r, c] = Q.front(); // Structured binding (C++17)
    Q.pop_front();
    if (SEEN.count({r, c}))
      continue;
    SEEN.insert({r, c});
    if (G[r][c] == 0) {
      ans++;
    }
    for (auto [dr, dc] : directions) { // Structured binding (C++17)
      int rr = r + dr, cc = c + dc;
      if (0 <= rr && rr < R && 0 <= cc && cc < C && G[rr][cc] == G[r][c] - 1) {
        Q.push_back({rr, cc});
      }
    }
  }

  return ans;
}

// Memoized function for calculating the number of ways to reach 0 starting from
// (r, c)
int ways(int r, int c) {
  if (G[r][c] == 0)
    return 1;
  string key = to_string(r) + "," + to_string(c);
  if (DP.find(key) != DP.end())
    return DP[key];

  int ans = 0;
  for (auto [dr, dc] : directions) { // Structured binding (C++17)
    int rr = r + dr, cc = c + dc;
    if (0 <= rr && rr < R && 0 <= cc && cc < C && G[rr][cc] == G[r][c] - 1) {
      ans += ways(rr, cc);
    }
  }
  DP[key] = ans;
  return ans;
}

int main(int argc, char *argv[]) {
  string infile = (argc >= 2) ? argv[1] : "10.in";

  // Read input file
  ifstream file(infile); // Include <fstream> for this line
  string line;
  vector<string> G_raw;

  while (getline(file, line)) {
    G_raw.push_back(line);
  }

  // Convert the grid from strings to integers
  R = G_raw.size();
  C = G_raw[0].size();
  G.resize(R, vector<int>(C));

  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      G[i][j] = G_raw[i][j] - '0';
    }
  }

  int p1 = 0, p2 = 0;

  // Iterate through the grid
  for (int r = 0; r < R; r++) {
    for (int c = 0; c < C; c++) {
      if (G[r][c] == 9) {
        p1 += ways1(r, c);
        p2 += ways(r, c);
      }
    }
  }

  // Output the results
  pr(p1);
  pr(p2);

  return 0;
}
