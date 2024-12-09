#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

void pr(int s) { cout << s << endl; }

int main(int argc, char *argv[]) {
  // Set input file name
  string infile = (argc >= 2) ? argv[1] : "6.in";

  ifstream file(infile);
  string line;
  vector<string> G;

  // Read the input file
  while (getline(file, line)) {
    G.push_back(line);
  }

  int R = G.size();
  int C = G[0].size();

  // Find starting position
  int sr, sc;
  for (int r = 0; r < R; ++r) {
    for (int c = 0; c < C; ++c) {
      if (G[r][c] == '^') {
        sr = r;
        sc = c;
        break;
      }
    }
  }

  int p1 = 0, p2 = 0;

  // Iterate over all possible exit points
  for (int o_r = 0; o_r < R; ++o_r) {
    for (int o_c = 0; o_c < C; ++o_c) {
      int r = sr, c = sc;
      int d = 0; // 0=up, 1=right, 2=down, 3=left
      set<tuple<int, int, int>> SEEN;
      set<pair<int, int>> SEEN_RC;

      while (true) {
        if (SEEN.count(make_tuple(r, c, d))) {
          p2 += 1;
          break;
        }
        SEEN.insert(make_tuple(r, c, d));
        SEEN_RC.insert({r, c});

        int dr[] = {-1, 0, 1, 0};
        int dc[] = {0, 1, 0, -1};
        int rr = r + dr[d];
        int cc = c + dc[d];

        if (!(0 <= rr && rr < R && 0 <= cc && cc < C)) {
          if (G[o_r][o_c] == '#') {
            p1 = SEEN_RC.size();
          }
          break;
        }

        if (G[rr][cc] == '#' || (rr == o_r && cc == o_c)) {
          d = (d + 1) % 4;
        } else {
          r = rr;
          c = cc;
        }
      }
    }
  }

  pr(p1);
  pr(p2);

  return 0;
}
