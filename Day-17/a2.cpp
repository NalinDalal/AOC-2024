#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

// Function to simulate the 3-bit computer
bool runProgram(int64_t initialA, const vector<int> &program) {
  // Registers A, B, and C
  int64_t A = initialA, B = 0, C = 0;
  size_t ip = 0; // Instruction pointer

  vector<int> output;

  while (ip < program.size()) {
    int opcode = program[ip];
    int operand = program[ip + 1];
    ip += 2; // Advance instruction pointer (unless overridden)

    switch (opcode) {
    case 0: { // adv: A = A / (2^operand)
      int64_t divisor = 1LL << operand;
      A /= divisor;
      break;
    }
    case 1: { // bxl: B = B XOR operand
      B ^= operand;
      break;
    }
    case 2: { // bst: B = operand % 8
      if (operand <= 3)
        B = operand;
      else if (operand == 4)
        B = A;
      else if (operand == 5)
        B = B;
      else if (operand == 6)
        B = C;
      break;
    }
    case 3: { // jnz: if A != 0, jump to operand
      if (A != 0)
        ip = operand;
      break;
    }
    case 4: { // bxc: B = B XOR C
      B ^= C;
      break;
    }
    case 5: { // out: output = operand % 8
      int value;
      if (operand <= 3)
        value = operand;
      else if (operand == 4)
        value = A;
      else if (operand == 5)
        value = B;
      else if (operand == 6)
        value = C;
      output.push_back(value % 8);
      break;
    }
    case 6: { // bdv: B = A / (2^operand)
      int64_t divisor = 1LL << operand;
      B = A / divisor;
      break;
    }
    case 7: { // cdv: C = A / (2^operand)
      int64_t divisor = 1LL << operand;
      C = A / divisor;
      break;
    }
    default:
      cerr << "Invalid opcode encountered!" << endl;
      return false;
    }
  }

  // Check if the output matches the program
  vector<int> expected(program.begin(), program.end());
  return output == expected;
}

int main() {
  // Input program
  vector<int> program = {0, 3, 5, 4, 3, 0};

  // Brute-force search for the lowest positive initial A
  int64_t initialA = 1;
  while (true) {
    if (runProgram(initialA, program)) {
      cout << "Lowest initial value for register A: " << initialA << endl;
      break;
    }
    ++initialA;
  }

  return 0;
}
