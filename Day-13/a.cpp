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

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Global variables
long long p1 = 0, p2 = 0;

// Solve function with memoization
long long solve(int ax, int ay, int bx, int by, int px, int py, bool part2) {
  long long P2 = part2 ? 10000000000000LL : 0;

  pair<double, vector<long long>> best = {numeric_limits<double>::max(), {}};

  for (int t1 = 0; t1 < 600; t1++) {
    for (int t2 = 0; t2 < 600; t2++) {
      long long cost = 3LL * t1 + t2;
      long long dx = ax * t1 + bx * t2;
      long long dy = ay * t1 + by * t2;

      if (dx == py && dy == py && dx > 0) {
        double score = static_cast<double>(dx) / cost;

        if (score < best.first) {
          best = {score, {t1, t2, cost, dx}};
        }
      }
    }
  }

  if (best.second.empty())
    return 0;

  long long t1 = best.second[0];
  long long t2 = best.second[1];
  long long cost = best.second[2];
  long long dx = best.second[3];

  long long amt = (P2 - 40000) / dx;

  map<pair<long long, long long>, long long> DP;

  function<long long(long long, long long)> f = [&](long long x, long long y) {
    // Memoization
    auto key = make_pair(x, y);
    if (DP.count(key))
      return DP[key];

    // Base cases
    if (x == 0 && y == 0)
      return 0LL;
    if (x < 0 || y < 0)
      return 10000000000000LL;

    // Recursive case
    long long ans = min(3 + f(x - ax, y - ay), 1 + f(x - bx, y - by));

    // Memoize and return
    return DP[key] = ans;
  };

  long long ans = f(px + P2 - amt * dx, py + P2 - amt * dx);

  if (ans < 1000000000000000LL) {
    return ans + amt * cost;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  // Input file handling
  string infile = argc >= 2 ? argv[1] : "13.in";

  // Read input
  ifstream file(infile);
  string input, line;
  string whole_input;
  while (getline(file, line)) {
    whole_input += line + "\n";
  }

  // Split input into machines
  vector<string> machines;
  size_t pos = 0;
  while ((pos = whole_input.find("\n\n")) != string::npos) {
    machines.push_back(whole_input.substr(0, pos));
    whole_input.erase(0, pos + 2);
  }
  machines.push_back(whole_input);

  // Process each machine
  for (const string &machine : machines) {
    istringstream iss(machine);
    string a_line, b_line, prize_line;

    getline(iss, a_line);
    getline(iss, b_line);
    getline(iss, prize_line);

    // Parse A button
    int ax = stoi(a_line.substr(a_line.find("+") + 1,
                                a_line.find(",") - a_line.find("+") - 1));
    int ay = stoi(a_line.substr(a_line.rfind("+") + 1,
                                a_line.length() - a_line.rfind("+") - 1));

    // Parse B button
    int bx = stoi(b_line.substr(b_line.find("+") + 1,
                                b_line.find(",") - b_line.find("+") - 1));
    int by = stoi(b_line.substr(b_line.rfind("+") + 1,
                                b_line.length() - b_line.rfind("+") - 1));

    // Parse prize coordinates
    int px = stoi(
        prize_line.substr(prize_line.find("X=") + 2,
                          prize_line.find(",") - prize_line.find("X=") - 2));
    int py = stoi(prize_line.substr(prize_line.find("Y=") + 2));

    // Solve for part 1 and part 2
    p1 += solve(ax, ay, bx, by, px, py, false);
    p2 += solve(ax, ay, bx, by, px, py, true);
  }

  // Print results
  cout << p1 << endl;
  cout << p2 << endl;

  return 0;
}
