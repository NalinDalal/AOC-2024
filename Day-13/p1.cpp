#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Point {
  int x, y;
};

// Parse a single block of input data
void parse(const string &block, Point &a, Point &b, Point &p) {
  regex re_a("Button A: X\\+(\\d+), Y\\+(\\d+)");
  regex re_b("Button B: X\\+(\\d+), Y\\+(\\d+)");
  regex re_p("Prize: X=(\\d+), Y=(\\d+)");

  smatch match;

  stringstream ss(block);
  string line;

  getline(ss, line);
  if (regex_search(line, match, re_a)) {
    a.x = stoi(match[1]);
    a.y = stoi(match[2]);
  }

  getline(ss, line);
  if (regex_search(line, match, re_b)) {
    b.x = stoi(match[1]);
    b.y = stoi(match[2]);
  }

  getline(ss, line);
  if (regex_search(line, match, re_p)) {
    p.x = stoi(match[1]);
    p.y = stoi(match[2]);
  }
}

// Check if the given i, j satisfy the equation for prize point
bool test(int i, int j, const Point &a, const Point &b, const Point &p) {
  int x = a.x * i + b.x * j;
  int y = a.y * i + b.y * j;
  return x == p.x && y == p.y;
}

int main() {
  ifstream fin("./13.in");
  if (!fin.is_open()) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  string content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
  fin.close();

  vector<string> blocks;
  size_t pos = 0, prev = 0;
  while ((pos = content.find("\n\n", prev)) != string::npos) {
    blocks.push_back(content.substr(prev, pos - prev));
    prev = pos + 2; // Skip "\n\n"
  }
  blocks.push_back(content.substr(prev));

  vector<int> prices;

  for (const auto &block : blocks) {
    Point a, b, p;
    parse(block, a, b, p);

    int va = INT_MAX;

    for (int i = 0; i < 100; ++i) {
      for (int j = 0; j < 100; ++j) {
        if (test(i, j, a, b, p)) {
          va = min(va, 3 * i + j);
        }
      }
    }

    if (va < INT_MAX) {
      prices.push_back(va);
    }
  }

  int spent = accumulate(prices.begin(), prices.end(), 0);
  cout << spent << endl;

  return 0;
}
