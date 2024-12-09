/* --- Day 7: Bridge Repair ---
The Historians take you to a familiar rope bridge over a river in the middle of
a jungle. The Chief isn't on this side of the bridge, though; maybe he's on the
other side?

When you go to cross the bridge, you notice a group of engineers trying to
repair it. (Apparently, it breaks pretty frequently.) You won't be able to cross
until it's fixed.

You ask how long it'll take; the engineers tell you that it only needs final
calibrations, but some young elephants were playing nearby and stole all the
operators from their calibration equations! They could finish the calibrations
if only someone could determine which test values could possibly be produced by
placing any combination of operators into their calibration equations (your
puzzle input).

For example:

190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20
Each line represents a single equation. The test value appears before the colon
on each line; it is your job to determine whether the remaining numbers can be
combined with operators to produce the test value.

Operators are always evaluated left-to-right, not according to precedence rules.
Furthermore, numbers in the equations cannot be rearranged. Glancing into the
jungle, you can see elephants holding two different types of operators: add (+)
and multiply (*).

Only three of the above equations can be made true by inserting operators:

190: 10 19 has only one position that accepts an operator: between 10 and 19.
Choosing + would give 29, but choosing * would give the test value (10 * 19 =
190). 3267: 81 40 27 has two positions for operators. Of the four possible
configurations of the operators, two cause the right side to match the test
value: 81 + 40 * 27 and 81 * 40 + 27 both equal 3267 (when evaluated
left-to-right)! 292: 11 6 16 20 can be solved in exactly one way: 11 + 6 * 16
+ 20. The engineers just need the total calibration result, which is the sum of
the test values from just the equations that could possibly be true. In the
above example, the sum of the test values for the three equations listed above
is 3749.

Determine which equations could possibly be true. What is their total
calibration result?

Your puzzle answer was 945512582195.

--- Part Two ---
The engineers seem concerned; the total calibration result you gave them is
nowhere close to being within safety tolerances. Just then, you spot your
mistake: some well-hidden elephants are holding a third type of operator.

The concatenation operator (||) combines the digits from its left and right
inputs into a single number. For example, 12 || 345 would become 12345. All
operators are still evaluated left-to-right.

Now, apart from the three equations that could be made true using only addition
and multiplication, the above example has three more equations that can be made
true by inserting operators:

156: 15 6 can be made true through a single concatenation: 15 || 6 = 156.
7290: 6 8 6 15 can be made true using 6 * 8 || 6 * 15.
192: 17 8 14 can be made true using 17 || 8 + 14.
Adding up all six test values (the three that could be made before using only +
and * plus the new three that can now be made by also using ||) produces the new
total calibration result of 11387.

Using your new knowledge of elephant hiding spots, determine which equations
could possibly be true. What is their total calibration result?

Your puzzle answer was 271691107779347.
 * */
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int p1 = 0, p2 = 0;

bool is_valid(int target, vector<int> &ns, bool p2) {
  if (ns.size() == 1) {
    return ns[0] == target;
  }

  vector<int> temp1 = {ns[0] + ns[1]};
  vector<int> temp2 = {ns[0] * ns[1]};

  if (is_valid(target, vector<int>(temp1.begin(), temp1.end()), p2)) {
    return true;
  }

  if (is_valid(target, vector<int>(temp2.begin(), temp2.end()), p2)) {
    return true;
  }

  if (p2) {
    string combined = to_string(ns[0]) + to_string(ns[1]);
    vector<int> temp3 = {stoi(combined)};
    if (is_valid(target, vector<int>(temp3.begin(), temp3.end()), p2)) {
      return true;
    }
  }

  return false;
}

void pr(int s) {
  cout << s << endl;
  // Replace with clipboard copy functionality if available in C++
}

int main(int argc, char *argv[]) {
  string infile = (argc >= 2) ? argv[1] : "7.in";
  ifstream file(infile);
  stringstream buffer;
  buffer << file.rdbuf();
  string D = buffer.str();

  string line;
  stringstream ss(D);

  while (getline(ss, line)) {
    stringstream line_ss(line);
    string target_str, ns_str;

    getline(line_ss, target_str, ':');
    getline(line_ss, ns_str);

    int target = stoi(target_str);
    vector<int> ns;
    stringstream ns_ss(ns_str);
    string temp;

    while (getline(ns_ss, temp, ' ')) {
      ns.push_back(stoi(temp));
    }

    if (is_valid(target, ns, false)) {
      p1 += target;
    }

    if (is_valid(target, ns, true)) {
      p2 += target;
    }
  }

  pr(p1);
  pr(p2);

  return 0;
}
