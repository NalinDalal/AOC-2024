/* --- Part Two ---
As you scan through the corrupted memory, you notice that some of the
conditional statements are also still intact. If you handle some of the
uncorrupted conditional statements in the program, you might be able to get an
even more accurate result.

There are two new instructions you'll need to handle:

The do() instruction enables future mul instructions.
The don't() instruction disables future mul instructions.
Only the most recent do() or don't() instruction applies. At the beginning of
the program, mul instructions are enabled.

For example:

xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))
This corrupted memory is similar to the example from before, but this time the
mul(5,5) and mul(11,8) instructions are disabled because there is a don't()
instruction before them. The other mul instructions function normally, including
the one at the end that gets re-enabled by a do() instruction.

This time, the sum of the results is 48 (2*4 + 8*5).

Handle the new instructions; what do you get if you add up all of the results of
just the enabled multiplications?*/
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

long long part2(const std::vector<std::string> &input) {
  long long result = 0;
  bool currentDo = true; // Tracks whether mul instructions are enabled

  // Regex to match mul(X,Y)
  std::regex mul_regex(R"(mul\(\d{1,3},\d{1,3}\))");
  // Regex to extract numbers from mul(X,Y)
  std::regex mul_numbers_regex(R"(mul\((\d{1,3}),(\d{1,3})\))");

  // Process each row in the input
  for (std::string row : input) {
    while (!row.empty()) {
      // Find positions of do(), don't(), and next mul(X,Y)
      size_t nextDo = row.find("do()");
      size_t nextDont = row.find("don't()");
      std::smatch mul_match;

      // Find the next mul(X,Y) match
      bool hasMul = std::regex_search(row, mul_match, mul_regex);
      size_t nextMul = hasMul ? row.find(mul_match.str(0)) : std::string::npos;

      // If no valid mul instruction is left, exit the loop
      if (!hasMul) {
        break;
      }

      // Handle do() instruction
      if ((nextDont == std::string::npos || (nextDo < nextDont)) &&
          nextDo < nextMul && nextDo != std::string::npos) {
        currentDo = true;
        row = row.substr(nextDo + 4); // Remove processed part
        continue;
      }

      // Handle don't() instruction
      if ((nextDo == std::string::npos || (nextDont < nextDo)) &&
          nextDont < nextMul && nextDont != std::string::npos) {
        currentDo = false;
        row = row.substr(nextDont + 8); // Remove processed part
        continue;
      }

      // Handle mul(X,Y) instruction
      if (currentDo) {
        std::smatch numbers_match;
        if (std::regex_match(mul_match.str(0), numbers_match,
                             mul_numbers_regex)) {
          int num1 = std::stoi(numbers_match[1].str());
          int num2 = std::stoi(numbers_match[2].str());
          result += num1 * num2; // Add product to result
        }
      }

      // Remove processed mul(X,Y) part
      row = row.substr(nextMul + mul_match.str(0).length());

      // Break if no further instructions remain
      if (nextDo == std::string::npos && nextDont == std::string::npos &&
          nextMul == std::string::npos) {
        break;
      }
    }
  }
  return result;
}

int main() {
  // Example usage
  std::ifstream input_file("p1.txt");
  if (!input_file.is_open()) {
    std::cerr << "Error: Could not open file p1.txt" << std::endl;
    return 1;
  }

  std::vector<std::string> input;
  std::string line;
  while (std::getline(input_file, line)) {
    input.push_back(line);
  }
  input_file.close();

  long long result = part2(input);
  std::cout << "Total sum of valid multiplications: " << result << std::endl;

  return 0;
}
