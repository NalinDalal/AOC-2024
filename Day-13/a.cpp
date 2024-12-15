/*
 * Day 13: Claw Contraption ---
Next up: the lobby of a resort on a tropical island. The Historians take a
moment to admire the hexagonal floor tiles before spreading out.

Fortunately, it looks like the resort has a new arcade! Maybe you can win some
prizes from the claw machines?

The claw machines here are a little unusual. Instead of a joystick or
directional buttons to control the claw, these machines have two buttons labeled
A and B. Worse, you can't just put in a token and play; it costs 3 tokens to
push the A button and 1 token to push the B button.

With a little experimentation, you figure out that each machine's buttons are
configured to move the claw a specific amount to the right (along the X axis)
and a specific amount forward (along the Y axis) each time that button is
pressed.

Each machine contains one prize; to win the prize, the claw must be positioned
exactly above the prize on both the X and Y axes.

You wonder: what is the smallest number of tokens you would have to spend to win
as many prizes as possible? You assemble a list of every machine's button
behavior and prize location (your puzzle input). For example:

Button A: X+94, Y+34
Button B: X+22, Y+67
Prize: X=8400, Y=5400

Button A: X+26, Y+66
Button B: X+67, Y+21
Prize: X=12748, Y=12176

Button A: X+17, Y+86
Button B: X+84, Y+37
Prize: X=7870, Y=6450

Button A: X+69, Y+23
Button B: X+27, Y+71
Prize: X=18641, Y=10279
This list describes the button configuration and prize location of four
different claw machines.

For now, consider just the first claw machine in the list:

Pushing the machine's A button would move the claw 94 units along the X axis and
34 units along the Y axis. Pushing the B button would move the claw 22 units
along the X axis and 67 units along the Y axis. The prize is located at X=8400,
Y=5400; this means that from the claw's initial position, it would need to move
exactly 8400 units along the X axis and exactly 5400 units along the Y axis to
be perfectly aligned with the prize in this machine. The cheapest way to win the
prize is by pushing the A button 80 times and the B button 40 times. This would
line up the claw along the X axis (because 80*94 + 40*22 = 8400) and along the Y
axis (because 80*34 + 40*67 = 5400). Doing this would cost 80*3 tokens for the A
presses and 40*1 for the B presses, a total of 280 tokens.

For the second and fourth claw machines, there is no combination of A and B
presses that will ever win a prize.

For the third claw machine, the cheapest way to win the prize is by pushing the
A button 38 times and the B button 86 times. Doing this would cost a total of
200 tokens.

So, the most prizes you could possibly win is two; the minimum tokens you would
have to spend to win all (two) prizes is 480.

You estimate that each button would need to be pressed no more than 100 times to
win a prize. How else would someone be expected to play?

Figure out how to win as many prizes as possible. What is the fewest tokens you
would have to spend to win all possible prizes?

*/
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <z3++.h>

using namespace std;

// Utility function to print and copy to clipboard
void pr(const string &s) {
  cout << s << endl;
  // Copying to clipboard can be handled with an external library (like
  // pyperclip in Python) For now, we will just print the output
}

vector<int> ints(const string &s) {
  vector<int> result;
  regex re("\\d+");
  auto words_begin = sregex_iterator(s.begin(), s.end(), re);
  auto words_end = sregex_iterator();

  for (auto it = words_begin; it != words_end; ++it) {
    result.push_back(stoi(it->str()));
  }

  return result;
}

long long solve(int ax, int ay, int bx, int by, int px, int py, bool part2) {
  long long P2 = part2 ? 10000000000000LL : 0;
  px += P2;
  py += P2;

  z3::context c;
  z3::solver solver(c);

  z3::expr t1 = c.int_const("t1");
  z3::expr t2 = c.int_const("t2");

  solver.add(t1 > 0);
  solver.add(t2 > 0);
  solver.add(t1 * ax + t2 * bx == px);
  solver.add(t1 * ay + t2 * by == py);

  if (solver.check() == z3::sat) {
    z3::model m = solver.get_model();
    long long ret = m.eval(3 * t1 + t2).as_int64();
    return ret;
  } else {
    return 0;
  }
}

int main(int argc, char *argv[]) {
  string infile = (argc >= 2) ? argv[1] : "13.in";
  ifstream file(infile);
  if (!file) {
    cerr << "Error opening file: " << infile << endl;
    return 1;
  }

  string D((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
  file.close();

  long long p1 = 0, p2 = 0;
  stringstream ss(D);
  string machine;

  while (getline(ss, machine, '\n')) {
    vector<int> values = ints(machine);
    int ax = values[0], ay = values[1], bx = values[2], by = values[3];
    int px = values[4], py = values[5];

    p1 += solve(ax, ay, bx, by, px, py, false);
    p2 += solve(ax, ay, bx, by, px, py, true);
  }

  pr(to_string(p1));
  pr(to_string(p2));

  return 0;
}
