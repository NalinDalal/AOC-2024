/* --- Part Two ---
The Elf looks quizzically at you. Did you misunderstand the assignment?

Looking for the instructions, you flip over the word search to find that this
isn't actually an XMAS puzzle; it's an X-MAS puzzle in which you're supposed to
find two MAS in the shape of an X. One way to achieve that is like this:

M.S
.A.
M.S

Irrelevant characters have again been replaced with . in the above diagram.
Within the X, each MAS can be written forwards or backwards. Here's the same
example from before, but this time all of the X-MASes have been kept instead:
.M.S......
..A..MSMS.
.M.S.MAA..
..A.ASMSM.
.M.S.M....
..........
S.S.S.S.S.
.A.A.A.A..
M.M.M.M.M.
..........

In this example, an X-MAS appears 9 times.

Flip the word search from the instructions back over to the word search side and
try again. How many times does an X-MAS appear?
*/

/* To solve Part Two, we now need to search for X-MAS patterns in the grid.
Specifically, we need to identify configurations where two occurrences of MAS
form an X shape. This means identifying the following structure:

M.S
.A.
M.S
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Function to check if an X-MAS exists centered at (row, col)
bool isXMAS(const vector<string> &grid, int row, int col) {
  int rows = grid.size();
  int cols = grid[0].size();

  // Check boundaries for the X-MAS pattern
  if (row - 1 < 0 || row + 1 >= rows || col - 1 < 0 || col + 1 >= cols)
    return false;

  // Extract characters for the pattern
  char topLeft = grid[row - 1][col - 1];
  char topRight = grid[row - 1][col + 1];
  char center = grid[row][col];
  char bottomLeft = grid[row + 1][col - 1];
  char bottomRight = grid[row + 1][col + 1];

  // Check for "MAS" or "SAM" in both diagonals
  bool diagonal1 = (topLeft == 'M' && center == 'A' && bottomRight == 'S') ||
                   (topLeft == 'S' && center == 'A' && bottomRight == 'M');
  bool diagonal2 = (topRight == 'M' && center == 'A' && bottomLeft == 'S') ||
                   (topRight == 'S' && center == 'A' && bottomLeft == 'M');

  return diagonal1 && diagonal2;
}

// Function to count X-MAS patterns in the grid
int countXMAS(const vector<string> &grid) {
  int count = 0;
  int rows = grid.size();
  int cols = grid[0].size();

  // Traverse every cell in the grid
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      if (isXMAS(grid, row, col)) {
        count++;
      }
    }
  }
  return count;
}

int main() {
  ifstream fin("p1.txt");
  if (!fin) {
    cerr << "Error opening file p1.txt" << endl;
    return 1;
  }

  vector<string> grid;
  string line;
  while (getline(fin, line)) {
    grid.push_back(line);
  }
  fin.close();

  int result = countXMAS(grid);
  cout << "Total occurrences of X-MAS: " << result << endl;

  return 0;
}
