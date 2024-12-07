#include <fstream>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

// Direction deltas: N, E, S, W
int dd[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// Function to determine if placing an obstacle causes a loop
bool will_loop(vector<vector<char>> &grid, int ii, int jj, int oi, int oj,
               int n, int m) {
  if (grid[oi][oj] == '#') {
    return false;
  }

  grid[oi][oj] = '#';
  int i = ii, j = jj;
  int dir = 0;
  set<tuple<int, int, int>> seen;

  while (true) {
    if (seen.count({i, j, dir})) {
      grid[oi][oj] = '.'; // Reset the obstacle
      return true;
    }
    seen.insert({i, j, dir});

    int next_i = i + dd[dir][0];
    int next_j = j + dd[dir][1];

    if (next_i < 0 || next_i >= n || next_j < 0 || next_j >= m) {
      grid[oi][oj] = '.'; // Reset the obstacle
      return false;
    }

    if (grid[next_i][next_j] == '#') {
      dir = (dir + 1) % 4; // Turn clockwise
    } else {
      i = next_i;
      j = next_j; // Move forward
    }
  }
}

int main() {
  // Read the grid from file
  ifstream fin("./day_06.in");
  if (!fin) {
    cerr << "Error: Unable to open file.\n";
    return 1;
  }

  vector<vector<char>> grid;
  string line;
  while (getline(fin, line)) {
    grid.push_back(vector<char>(line.begin(), line.end()));
  }
  fin.close();

  int n = grid.size();
  int m = grid[0].size();

  // Find the starting position marked as '^'
  int ii = -1, jj = -1;
  bool found = false;
  for (int i = 0; i < n && !found; ++i) {
    for (int j = 0; j < m; ++j) {
      if (grid[i][j] == '^') {
        ii = i;
        jj = j;
        found = true;
        break;
      }
    }
  }

  if (ii == -1 || jj == -1) {
    cerr << "Error: Starting position '^' not found.\n";
    return 1;
  }

  // Assess possible starting locations
  int dir = 0;
  set<pair<int, int>> og_seen;
  int i = ii, j = jj;

  while (true) {
    og_seen.insert({i, j});

    int next_i = i + dd[dir][0];
    int next_j = j + dd[dir][1];

    if (next_i < 0 || next_i >= n || next_j < 0 || next_j >= m) {
      break;
    }

    if (grid[next_i][next_j] == '#') {
      dir = (dir + 1) % 4; // Turn clockwise
    } else {
      i = next_i;
      j = next_j; // Move forward
    }
  }

  // Count loops
  int ans = 0;
  for (const auto &[oi, oj] : og_seen) {
    // Skip if the obstacle position is the same as the guard's position
    if (oi == ii && oj == jj) {
      continue;
    }
    if (will_loop(grid, ii, jj, oi, oj, n, m)) {
      ans++;
    }
  }

  cout << ans << "\n";
  return 0;
}
