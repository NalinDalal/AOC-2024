/* --- Day 2: Red-Nosed Reports ---
Fortunately, the first location The Historians want to search isn't a long walk
from the Chief Historian's office.

While the Red-Nosed Reindeer nuclear fusion/fission plant appears to contain no
sign of the Chief Historian, the engineers there run up to you as soon as they
see you. Apparently, they still talk about the time Rudolph was saved through
molecular synthesis from a single electron.

They're quick to add that - since you're already here - they'd really appreciate
your help analyzing some unusual data from the Red-Nosed reactor. You turn to
check if The Historians are waiting for you, but they seem to have already
divided into groups that are currently searching every corner of the facility.
You offer to help with the unusual data.

The unusual data (your puzzle input) consists of many reports, one report per
line. Each report is a list of numbers called levels that are separated by
spaces. For example:

7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9
This example data contains six reports each containing five levels.

The engineers are trying to figure out which reports are safe. The Red-Nosed
reactor safety systems can only tolerate levels that are either gradually
increasing or gradually decreasing. So, a report only counts as safe if both of
the following are true:

The levels are either all increasing or all decreasing.
Any two adjacent levels differ by at least one and at most three.
In the example above, the reports can be found safe or unsafe by checking those
rules:

7 6 4 2 1: Safe because the levels are all decreasing by 1 or 2.
1 2 7 8 9: Unsafe because 2 7 is an increase of 5.
9 7 6 2 1: Unsafe because 6 2 is a decrease of 4.
1 3 2 4 5: Unsafe because 1 3 is increasing but 3 2 is decreasing.
8 6 4 4 1: Unsafe because 4 4 is neither an increase or a decrease.
1 3 6 7 9: Safe because the levels are all increasing by 1, 2, or 3.
So, in this example, 2 reports are safe.

Analyze the unusual data from the engineers. How many reports are safe?
*/

// Approach:
//  helper function to each line(k input) to check if safe {1<=level_diff<=3}
//  else not safe
//  log into an array these safe or not safe
//  then count safe in the array
//  main function to count no of safe reports

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
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

// Main function to read from file and count the number of safe reports
int countSafeReportsFromFile(const std::string &filename) {
  std::ifstream file("p1.txt");
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
    if (isSafeReport(levels)) {
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
