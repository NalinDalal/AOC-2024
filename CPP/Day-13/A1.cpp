#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct ClawMachine {
  int ax, ay;         // Button A movements
  int bx, by;         // Button B movements
  int prizeX, prizeY; // Prize position
};

vector<ClawMachine> parseInput(const string &filename) {
  ifstream file(filename);
  string line;
  vector<ClawMachine> machines;

  while (getline(file, line)) {
    ClawMachine cm;
    if (line.empty())
      continue;

    // Parse Button A
    getline(file, line);
    sscanf(line.c_str(), "Button A: X+%d, Y+%d", &cm.ax, &cm.ay);

    // Parse Button B
    getline(file, line);
    sscanf(line.c_str(), "Button B: X+%d, Y+%d", &cm.bx, &cm.by);

    // Parse Prize
    getline(file, line);
    sscanf(line.c_str(), "Prize: X=%d, Y=%d", &cm.prizeX, &cm.prizeY);

    machines.push_back(cm);
    getline(file, line); // Skip empty line
  }

  return machines;
}

int findMinCost(const ClawMachine &cm, int maxPress = 100) {
  int minCost = INT_MAX;

  // Brute-force over all possible button presses
  for (int a = 0; a <= maxPress; ++a) {
    for (int b = 0; b <= maxPress; ++b) {
      int totalX = a * cm.ax + b * cm.bx;
      int totalY = a * cm.ay + b * cm.by;

      // Check if claw aligns with the prize
      if (totalX == cm.prizeX && totalY == cm.prizeY) {
        int cost = 3 * a + 1 * b; // Cost formula
        minCost = min(minCost, cost);
      }
    }
  }

  return minCost;
}

int main() {
  string filename = "13.in";
  vector<ClawMachine> machines = parseInput(filename);

  int totalCost = 0, prizesWon = 0;

  for (const auto &machine : machines) {
    int cost = findMinCost(machine);
    if (cost != INT_MAX) {
      totalCost += cost;
      ++prizesWon;
    }
  }

  cout << "Prizes Won: " << prizesWon << endl;
  cout << "Total Cost: " << totalCost << endl;

  return 0;
}
