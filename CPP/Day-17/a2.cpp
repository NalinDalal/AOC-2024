/* --- Part Two ---
Digging deeper in the device's manual, you discover the problem: this program is
supposed to output another copy of the program! Unfortunately, the value in
register A seems to have been corrupted. You'll need to find a new value to
which you can initialize register A so that the program's output instructions
produce an exact copy of the program itself.

For example:

Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0
This program outputs a copy of itself if register A is instead initialized to
117440. (The original initial value of register A, 2024, is ignored.)

What is the lowest positive initial value for register A that causes the program
to output a copy of itself?

*/

#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

// Custom hash function for pair<int, int>
struct pair_hash {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2> &pair) const {
    return hash<T1>()(pair.first) ^ (hash<T2>()(pair.second) << 1);
  }
};

// Helper function to split a string by a delimiter
vector<string> split(const string &s, char delimiter) {
  vector<string> tokens;
  string token;
  for (char c : s) {
    if (c == delimiter) {
      if (!token.empty())
        tokens.push_back(token);
      token.clear();
    } else {
      token += c;
    }
  }
  if (!token.empty())
    tokens.push_back(token);
  return tokens;
}

int main() {
  ifstream fin("./17.in");
  if (!fin.is_open()) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  vector<string> lines;
  string line;
  while (getline(fin, line)) {
    lines.push_back(line);
  }
  fin.close();

  if (lines.size() < 4) {
    cerr << "Insufficient input data." << endl;
    return 1;
  }

  int A = stoi(split(lines[0], ' ')[2]);
  int B = stoi(split(lines[1], ' ')[2]);
  int C = stoi(split(lines[2], ' ')[2]);

  vector<int> program;
  for (const string &num : split(split(lines[4], ' ')[1], ',')) {
    program.push_back(stoi(num));
  }

  unordered_map<int, set<int>> valid;
  unordered_map<pair<int, int>, set<pair<int, int>>, pair_hash> tree;

  auto get_compatible = [&](int cur, int bits, int prev) {
    set<pair<int, int>> compatible;
    for (int prev_bits : valid[prev]) {
      int overlap = 10 - (cur - prev) * 3;
      int prefix = prev_bits >> (10 - overlap);
      if (prefix == bits % (1 << overlap)) {
        compatible.emplace(prev, prev_bits);
      }
    }
    return compatible;
  };

  for (int cur = 0; cur < program.size(); ++cur) {
    valid[cur] = {};
    int out = program[cur];

    for (int bits = 0; bits < (1 << 10); ++bits) {
      tree[{cur, bits}] = {};
      int R = bits % 8;

      if ((R ^ (bits >> (R ^ 6)) ^ 2) % 8 == out) {
        for (int prev = cur - 3; prev < cur; ++prev) {
          if (prev < 0)
            continue;
          auto compatible = get_compatible(cur, bits, prev);
          tree[{cur, bits}].insert(compatible.begin(), compatible.end());
        }

        if (cur == 0 || !tree[{cur, bits}].empty()) {
          valid[cur].insert(bits);
        }
      }
    }
  }

  vector<int> ans;
  queue<tuple<int, int, int>> stack;

  for (int bits : valid[program.size() - 1]) {
    if (bits < 256) {
      stack.emplace(0, program.size() - 1, bits);
    }
  }

  while (!stack.empty()) {
    auto [A, cur, bits] = stack.front();
    stack.pop();

    A = (A << 3) ^ (bits % 8);

    if (cur == 0) {
      ans.push_back(A);
    }

    for (const auto &child : tree[{cur, bits}]) {
      if (child.first == cur - 1) {
        stack.emplace(A, child.first, child.second);
      }
    }
  }

  if (!ans.empty()) {
    cout << *min_element(ans.begin(), ans.end()) << endl;
  } else {
    cout << "No valid answer found." << endl;
  }

  return 0;
}
