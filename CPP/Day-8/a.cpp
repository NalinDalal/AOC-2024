#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Custom hash function for pair<int, int>
struct pair_hash {
  template <typename T1, typename T2>
  size_t operator()(const pair<T1, T2> &p) const {
    auto h1 = hash<T1>{}(p.first);
    auto h2 = hash<T2>{}(p.second);
    return h1 ^ h2; // Combine the hashes
  }
};

void pr(int s) { cout << s << endl; }

int main(int argc, char *argv[]) {
  // Set input file name
  string infile = (argc >= 2) ? argv[1] : "8.in";

  ifstream file(infile);
  string line;
  vector<string> G;

  // Read input file
  while (getline(file, line)) {
    G.push_back(line);
  }

  int R = G.size();
  int C = G[0].size();

  // Map for storing positions of characters
  unordered_map<char, vector<pair<int, int>>> P;

  // Fill P with positions of each character
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (G[r][c] != '.') {
        P[G[r][c]].push_back({r, c});
      }
    }
  }

  unordered_set<pair<int, int>, pair_hash> A1;
  unordered_set<pair<int, int>, pair_hash> A2;

  // Traverse through the grid and apply the logic
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      for (const auto &kv : P) {
        for (const auto &p1 : kv.second) {
          for (const auto &p2 : kv.second) {
            if (p1 != p2) {
              int r1 = p1.first, c1 = p1.second;
              int r2 = p2.first, c2 = p2.second;

              int d1 = abs(r - r1) + abs(c - c1);
              int d2 = abs(r - r2) + abs(c - c2);

              int dr1 = r - r1, dr2 = r - r2;
              int dc1 = c - c1, dc2 = c - c2;

              // Check if (r,c) (r1,c1) (r2,c2) are all on a line
              if ((d1 == 2 * d2 || d1 * 2 == d2) && 0 <= r && r < R && 0 <= c &&
                  c < C && dr1 * dc2 == dc1 * dr2) {
                A1.insert({r, c});
              }
              if (0 <= r && r < R && 0 <= c && c < C &&
                  dr1 * dc2 == dc1 * dr2) {
                A2.insert({r, c});
              }
            }
          }
        }
      }
    }
  }

  // Output the results
  pr(A1.size());
  pr(A2.size());

  return 0;
}
