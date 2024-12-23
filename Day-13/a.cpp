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
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

// Custom GCD function
long long gcd(long long a, long long b) {
  a = abs(a);
  b = abs(b);
  while (b != 0) {
    long long temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

// Extended Euclidean Algorithm
long long extended_gcd(long long a, long long b, long long &x, long long &y) {
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  long long x1, y1;
  long long d = extended_gcd(b, a % b, x1, y1);
  x = y1;
  y = x1 - y1 * (a / b);
  return d;
}

bool can_reach_target(long long ax, long long ay, long long bx, long long by,
                      long long px, long long py) {
  // Use GCD to check if solution exists
  long long gcd_x = gcd(abs(ax), abs(bx));
  long long gcd_y = gcd(abs(ay), abs(by));

  return (px % gcd_x == 0) && (py % gcd_y == 0);
}

tuple<long long, long long, long long>
solve_machine(long long ax, long long ay, long long bx, long long by,
              long long px, long long py, long long max_presses = 100) {

  if (!can_reach_target(ax, ay, bx, by, px, py)) {
    return make_tuple(-1, -1, LLONG_MAX);
  }

  for (long long a_presses = 0; a_presses <= max_presses; ++a_presses) {
    for (long long b_presses = 0; b_presses <= max_presses; ++b_presses) {
      if (a_presses * ax + b_presses * bx == px &&
          a_presses * ay + b_presses * by == py) {
        return make_tuple(a_presses, b_presses, a_presses * 3 + b_presses * 1);
      }
    }
  }

  return make_tuple(-1, -1, LLONG_MAX);
}

long long
solve_claw_machines(vector<tuple<long long, long long, long long, long long,
                                 long long, long long>> &machines) {
  vector<tuple<long long, long long, long long>> valid_machines;

  // Find valid machine solutions
  for (const auto &machine : machines) {
    auto solution =
        solve_machine(get<0>(machine), get<1>(machine), get<2>(machine),
                      get<3>(machine), get<4>(machine), get<5>(machine));

    if (get<2>(solution) != LLONG_MAX) {
      valid_machines.push_back(solution);
    }
  }

  // Try all prize combination strategies
  long long min_tokens = LLONG_MAX;
  for (int n_prizes = valid_machines.size(); n_prizes > 0; --n_prizes) {
    vector<bool> v(valid_machines.size(), false);
    fill(v.begin(), v.begin() + n_prizes, true);

    do {
      long long total_tokens = 0;
      for (size_t i = 0; i < valid_machines.size(); ++i) {
        if (v[i]) {
          total_tokens += get<2>(valid_machines[i]);
        }
      }
      min_tokens = min(min_tokens, total_tokens);
    } while (prev_permutation(v.begin(), v.end()));
  }

  return min_tokens;
}

int main() {
  ifstream input("a.in");

  vector<
      tuple<long long, long long, long long, long long, long long, long long>>
      machines;

  long long ax, ay, bx, by, px, py;
  while (input >> ax >> ay >> bx >> by >> px >> py) {
    input.ignore(100, ':'); // Ignore rest of line
    machines.emplace_back(ax, ay, bx, by, px, py);
  }

  long long result = solve_claw_machines(machines);
  cout << result << endl;

  return 0;
}
