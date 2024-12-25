#include <algorithm>
#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#define MAX_PROGRAM_SIZE 1000

int A, B, C;
std::vector<int> program;
std::map<std::pair<int, int>, std::set<std::pair<int, int>>> tree;
std::map<int, std::set<int>> valid;

// Function to safely convert a string to an integer
int safe_stoi(const std::string &str) {
  try {
    return std::stoi(str);
  } catch (const std::invalid_argument &e) {
    std::cerr << "Invalid argument: " << str
              << " could not be converted to integer" << std::endl;
    throw; // Rethrow to propagate the error
  } catch (const std::out_of_range &e) {
    std::cerr << "Out of range: " << str << " is out of range for an integer"
              << std::endl;
    throw; // Rethrow to propagate the error
  }
}

// Function to extract integer after "Register X: " pattern
int extract_register_value(const std::string &line) {
  size_t pos = line.find(": ");
  if (pos != std::string::npos) {
    return safe_stoi(line.substr(pos + 2)); // Extract the value after ": "
  }
  return 0; // Default value if the line is not in expected format
}

// Function to extract program instructions from the input string
void parse_program(const std::string &program_str) {
  size_t start_pos = program_str.find(" ") + 1;
  std::string program_part = program_str.substr(start_pos);
  size_t pos = 0;
  while ((pos = program_part.find(",")) != std::string::npos) {
    program.push_back(safe_stoi(program_part.substr(0, pos)));
    program_part.erase(0, pos + 1);
  }
  program.push_back(safe_stoi(program_part)); // Add the last number
}

int main() {
  // Read input file
  std::ifstream fin("./17.in");
  if (!fin.is_open()) {
    std::cerr << "Error opening file 'day_17.in'" << std::endl;
    return 1;
  }

  std::string line;
  std::vector<std::string> lines;
  while (std::getline(fin, line)) {
    lines.push_back(line);
  }
  fin.close();

  // Parse values A, B, C from the first 3 lines (Register X: value format)
  try {
    A = extract_register_value(lines[0]);
    B = extract_register_value(lines[1]);
    C = extract_register_value(lines[2]);
  } catch (const std::exception &e) {
    return 1; // Exit if parsing A, B, or C fails
  }

  // Parse program from the line starting with "Program: "
  parse_program(lines[4]);

  // Initialize valid and tree
  for (size_t cur = 0; cur < program.size(); ++cur) {
    valid[cur] = std::set<int>();
    int out = program[cur];

    for (int bits = 0; bits < (1 << 10); ++bits) {
      tree[{cur, bits}] = std::set<std::pair<int, int>>();
      int R = bits % 8;

      // Specific to the input, check the condition
      if ((R ^ (bits >> (R ^ 6)) ^ 2) % 8 == out) {
        // Check previous valid states
        for (int prev = std::max(0, int(cur) - 3); prev < cur; ++prev) {
          // Code to get compatible pairs (details depend on input)
          tree[{cur, bits}].insert({prev, bits}); // Simplified for example
        }

        if (cur == 0 || !tree[{cur, bits}].empty()) {
          valid[cur].insert(bits);
        }
      }
    }
  }

  // Follow the tree
  std::vector<int> possible_A;
  std::vector<std::tuple<int, int, int>> stack;

  for (int bits : valid[program.size() - 1]) {
    if (bits >= 256) {
      continue;
    }
    stack.push_back({0, int(program.size()) - 1, bits});
  }

  std::cout << "Stack initialized with " << stack.size() << " elements."
            << std::endl;

  std::vector<int> ans;
  while (!stack.empty()) {
    auto [A_current, cur, bits] = stack.back();
    stack.pop_back();
    A_current = (A_current << 3) ^ (bits % 8);

    if (cur == 0) {
      ans.push_back(A_current);
    }

    for (const auto &child : tree[{cur, bits}]) {
      if (child.first != cur - 1) {
        continue;
      }
      stack.push_back({A_current, child.first, child.second});
    }
  }

  // Output the smallest result
  if (!ans.empty()) {
    std::cout << *std::min_element(ans.begin(), ans.end()) << std::endl;
  } else {
    std::cerr << "No valid answer found." << std::endl;
  }

  return 0;
}
