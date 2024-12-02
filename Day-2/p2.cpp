/* --- Part Two ---
The engineers are surprised by the low number of safe reports until they realize
they forgot to tell you about the Problem Dampener.

The Problem Dampener is a reactor-mounted module that lets the reactor safety
systems tolerate a single bad level in what would otherwise be a safe report.
It's like the bad level never happened!

Now, the same rules apply as before, except if removing a single level from an
unsafe report would make it safe, the report instead counts as safe.

More of the above example's reports are now safe:

7 6 4 2 1: Safe without removing any level.
1 2 7 8 9: Unsafe regardless of which level is removed.
9 7 6 2 1: Unsafe regardless of which level is removed.
1 3 2 4 5: Safe by removing the second level, 3.
8 6 4 4 1: Safe by removing the third level, 4.
1 3 6 7 9: Safe without removing any level.
Thanks to the Problem Dampener, 4 reports are actually safe!

Update your analysis by handling situations where the Problem Dampener can
remove a single level from unsafe reports. How many reports are now safe?

*/

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// Helper function to check if a single report is safe
bool isSafeReport(const std::vector<int> &levels) {
  int n = levels.size();
  if (n < 2)
    return false; // A single level is not a valid report

  bool increasing = levels[1] > levels[0];
  for (size_t i = 1; i < n; ++i) {
    int level_diff = std::abs(levels[i] - levels[i - 1]);

    // Check if difference is within range
    if (level_diff < 1 || level_diff > 3)
      return false;

    // Check if all changes follow the same trend
    if ((levels[i] > levels[i - 1]) != increasing)
      return false;
  }

  return true;
}

// Helper function to check if removing one level makes the report safe
bool isSafeWithOneRemoval(const std::vector<int> &levels) {
  int n = levels.size();
  for (int i = 0; i < n; ++i) {
    std::vector<int> modified_levels;
    for (int j = 0; j < n; ++j) {
      if (j != i)
        modified_levels.push_back(levels[j]);
    }
    if (isSafeReport(modified_levels))
      return true;
  }
  return false;
}

// Main function to count safe reports considering the Problem Dampener
int countSafeReportsFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << "\n";
    return -1;
  }

  int safe_count = 0;
  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::vector<int> levels;
    int level;

    // Parse integers from the line
    while (ss >> level) {
      levels.push_back(level);
    }

    // Check if the report is safe
    if (isSafeReport(levels) || isSafeWithOneRemoval(levels)) {
      ++safe_count;
    }
  }

  file.close();
  return safe_count;
}

int main() {
  const std::string filename = "p1.txt";
  int safe_reports = countSafeReportsFromFile(filename);

  if (safe_reports != -1) { // Only output if the file was successfully read
    std::cout << "Number of safe reports: " << safe_reports << "\n";
  }

  return 0;
}
