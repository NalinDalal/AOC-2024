/* --- Day 17: Chronospatial Computer ---
The Historians push the button on their strange device, but this time, you all
just feel like you're falling.

"Situation critical", the device announces in a familiar voice. "Bootstrapping
process failed. Initializing debugger...."

The small handheld device suddenly unfolds into an entire computer! The
Historians look around nervously before one of them tosses it to you.

This seems to be a 3-bit computer: its program is a list of 3-bit numbers (0
through 7), like 0,1,2,3. The computer also has three registers named A, B, and
C, but these registers aren't limited to 3 bits and can instead hold any
integer.

The computer knows eight instructions, each identified by a 3-bit number (called
the instruction's opcode). Each instruction also reads the 3-bit number after it
as an input; this is called its operand.

A number called the instruction pointer identifies the position in the program
from which the next opcode will be read; it starts at 0, pointing at the first
3-bit number in the program. Except for jump instructions, the instruction
pointer increases by 2 after each instruction is processed (to move past the
instruction's opcode and its operand). If the computer tries to read an opcode
past the end of the program, it instead halts.

So, the program 0,1,2,3 would run the instruction whose opcode is 0 and pass it
the operand 1, then run the instruction having opcode 2 and pass it the operand
3, then halt.

There are two types of operands; each instruction specifies the type of its
operand. The value of a literal operand is the operand itself. For example, the
value of the literal operand 7 is the number 7. The value of a combo operand can
be found as follows:

Combo operands 0 through 3 represent literal values 0 through 3.
Combo operand 4 represents the value of register A.
Combo operand 5 represents the value of register B.
Combo operand 6 represents the value of register C.
Combo operand 7 is reserved and will not appear in valid programs.
The eight instructions are as follows:

The adv instruction (opcode 0) performs division. The numerator is the value in
the A register. The denominator is found by raising 2 to the power of the
instruction's combo operand. (So, an operand of 2 would divide A by 4 (2^2); an
operand of 5 would divide A by 2^B.) The result of the division operation is
truncated to an integer and then written to the A register.

The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the
instruction's literal operand, then stores the result in register B.

The bst instruction (opcode 2) calculates the value of its combo operand modulo
8 (thereby keeping only its lowest 3 bits), then writes that value to the B
register.

The jnz instruction (opcode 3) does nothing if the A register is 0. However, if
the A register is not zero, it jumps by setting the instruction pointer to the
value of its literal operand; if this instruction jumps, the instruction pointer
is not increased by 2 after this instruction.

The bxc instruction (opcode 4) calculates the bitwise XOR of register B and
register C, then stores the result in register B. (For legacy reasons, this
instruction reads an operand but ignores it.)

The out instruction (opcode 5) calculates the value of its combo operand modulo
8, then outputs that value. (If a program outputs multiple values, they are
separated by commas.)

The bdv instruction (opcode 6) works exactly like the adv instruction except
that the result is stored in the B register. (The numerator is still read from
the A register.)

The cdv instruction (opcode 7) works exactly like the adv instruction except
that the result is stored in the C register. (The numerator is still read from
the A register.)

Here are some examples of instruction operation:

If register C contains 9, the program 2,6 would set register B to 1.
If register A contains 10, the program 5,0,5,1,5,4 would output 0,1,2.
If register A contains 2024, the program 0,1,5,4,3,0 would output
4,2,5,6,7,7,7,7,3,1,0 and leave 0 in register A. If register B contains 29, the
program 1,7 would set register B to 26. If register B contains 2024 and register
C contains 43690, the program 4,0 would set register B to 44354. The Historians'
strange device has finished initializing its debugger and is displaying some
information about the program it is trying to run (your puzzle input). For
example:

Register A: 729
Register B: 0
Register C: 0

Program: 0,1,5,4,3,0
Your first task is to determine what the program is trying to output. To do
this, initialize the registers to the given values, then run the given program,
collecting any output produced by out instructions. (Always join the values
produced by out instructions with commas.) After the above program halts, its
final output will be 4,6,3,5,6,3,5,2,1,0.

Using the information provided by the debugger, initialize the registers to the
given values, then run the program. Once it halts, what do you get if you use
commas to join the values it output into a single string?

Your puzzle answer was 1,3,5,1,7,2,5,1,6.

The first half of this puzzle is complete! It provides one gold star: *

--- Part Two ---
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
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
// Global variables to mimic Python script's structure
int A, B, C;
std::vector<int> program;
// Function to extract integers from a string
std::vector<int> ints(const std::string &s) {
  std::vector<int> result;
  std::regex re("-?\\d+");
  std::sregex_iterator it(s.begin(), s.end(), re);
  std::sregex_iterator end;
  while (it != end) {
    result.push_back(std::stoi(it->str()));
    ++it;
  }
  return result;
}
// Modified run function to match Python behavior
std::vector<int> run(int Ast, bool part2) {
  auto getCombo = [](int x) {
    if (x >= 0 && x <= 3)
      return x;
    if (x == 4)
      return A;
    if (x == 5)
      return B;
    if (x == 6)
      return C;
    return -1;
  };
  A = Ast;
  B = 0;
  C = 0;
  size_t ip = 0;
  std::vector<int> out;
  while (true) {
    if (ip >= program.size())
      return out;
    int cmd = program[ip];
    int op = program[ip + 1];
    int combo = getCombo(op);
    switch (cmd) {
    case 0: // Division
      A = A / static_cast<int>(std::pow(2, combo));
      ip += 2;
      break;
    case 1: // XOR
      B = B ^ op;
      ip += 2;
      break;
    case 2: // Modulo
      B = combo % 8;
      ip += 2;
      break;
    case 3: // Conditional jump
      if (A != 0) {
        ip = op;
      } else {
        ip += 2;
      }
      break;
    case 4: // XOR with C
      B = B ^ C;
      ip += 2;
      break;
    case 5: // Output
      out.push_back(combo % 8);
      if (part2 && out.back() != program[out.size() - 1]) {
        return out;
      }
      ip += 2;
      break;
    case 6: // Division
      B = A / static_cast<int>(std::pow(2, combo));
      ip += 2;
      break;
    case 7: // Division
      C = A / static_cast<int>(std::pow(2, combo));
      ip += 2;
      break;
    }
  }
}
int main(int argc, char *argv[]) {
  // Default input file or from command line
  std::string infile = (argc >= 2) ? argv[1] : "17.in";
  std::ifstream file(infile);
  std::string content((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  // Split content
  size_t delimiter = content.find("\n\n");
  std::string regs_str = content.substr(0, delimiter);
  std::string program_str = content.substr(delimiter + 2);
  // Parse registers
  std::vector<int> reg_vals = ints(regs_str);
  A = reg_vals[0];
  B = reg_vals[1];
  C = reg_vals[2];
  // Parse program
  program_str = program_str.substr(program_str.find(":") + 1);
  std::replace(program_str.begin(), program_str.end(), ',', ' ');
  std::istringstream iss(program_str);
  int val;
  while (iss >> val) {
    program.push_back(val);
  }
  // Part 1
  std::vector<int> part1 = run(A, false);

  // Print Part 1 results
  std::cout << "Part 1: ";
  for (size_t i = 0; i < part1.size(); ++i) {
    std::cout << part1[i] << (i < part1.size() - 1 ? "," : "\n");
  }
  // Part 2
  int Ast = 0;
  int best = 0;
  while (true) {
    ++Ast;
    A = Ast * std::pow(8, 9) + 0676236017;
    std::vector<int> out = run(A, true);

    if (out == program) {
      std::cout << "Part 2: " << A << std::endl;
      break;
    } else if (out.size() > best) {
      best = out.size();
    }
  }
  return 0;
}
