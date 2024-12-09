#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Helper function to print and copy results
void pr(long long result) { cout << result << endl; }

// Global variable to store input
string D;

// Function to solve the problem
long long solve(bool part2) {
  deque<tuple<int, int, int>> A; // Stores (position, size, file_id) for files
  deque<pair<int, int>> SPACE;   // Stores (position, size) for free spaces
  vector<int> FINAL;             // Final representation of the disk
  int file_id = 0;
  int pos = 0;

  // Parse the input
  for (size_t i = 0; i < D.size(); ++i) {
    char c = D[i];
    if (i % 2 == 0) {
      int size = c - '0';
      if (part2) {
        A.push_back(make_tuple(pos, size, file_id));
      }
      for (int j = 0; j < size; ++j) {
        FINAL.push_back(file_id);
        if (!part2) {
          A.push_back(make_tuple(pos, 1, file_id));
        }
        pos++;
      }
      file_id++;
    } else {
      int size = c - '0';
      SPACE.push_back(make_pair(pos, size));
      for (int j = 0; j < size; ++j) {
        FINAL.push_back(-1); // Use -1 for free space
        pos++;
      }
    }
  }

  // Perform the compacting process
  for (auto it = A.rbegin(); it != A.rend(); ++it) {
    int pos = get<0>(*it);
    int sz = get<1>(*it);
    int file_id = get<2>(*it);

    for (size_t space_i = 0; space_i < SPACE.size(); ++space_i) {
      int space_pos = SPACE[space_i].first;
      int space_sz = SPACE[space_i].second;

      if (space_pos < pos && sz <= space_sz) {
        for (int i = 0; i < sz; ++i) {
          assert(FINAL[pos + i] == file_id);
          FINAL[pos + i] = -1;            // Mark as free space
          FINAL[space_pos + i] = file_id; // Move file block
        }
        SPACE[space_i] = make_pair(space_pos + sz, space_sz - sz);
        break;
      }
    }
  }

  // Calculate the checksum
  long long ans = 0;
  for (size_t i = 0; i < FINAL.size(); ++i) {
    if (FINAL[i] != -1) {
      ans += i * FINAL[i];
    }
  }

  return ans;
}

int main(int argc, char *argv[]) {
  // Read input file
  string infile = (argc >= 2) ? argv[1] : "9.in";
  ifstream inputFile(infile);
  if (!inputFile) {
    cerr << "Error: Could not open input file." << endl;
    return 1;
  }

  getline(inputFile, D);
  inputFile.close();

  // Solve for part 1 and part 2
  long long p1 = solve(false);
  long long p2 = solve(true);

  // Print results
  pr(p1);
  pr(p2);

  return 0;
}
