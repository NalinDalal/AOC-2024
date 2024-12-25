/* --- Day 4: Ceres Search ---
"Looks like the Chief's not here. Next!" One of The Historians pulls out a
device and pushes the only button on it. After a brief flash, you recognize the
interior of the Ceres monitoring station!

As the search for the Chief continues, a small Elf who lives on the station tugs
on your shirt; she'd like to know if you could help her with her word search
(your puzzle input). She only has to find one word: XMAS.

This word search allows words to be horizontal, vertical, diagonal, written
backwards, or even overlapping other words. It's a little unusual, though, as
you don't merely need to find one instance of XMAS - you need to find all of
them. Here are a few ways XMAS might appear, where irrelevant characters have
been replaced with .:


..X...
.SAMX.
.A..A.
XMAS.S
.X....
The actual word search will be full of letters instead. For example:

MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX

In this word search, XMAS occurs a total of 18 times; here's the same word
search again, but where letters not involved in any XMAS have been replaced with
.:

....XXMAS.
.SAMXMS...
...S..A...
..A.A.MS.X
XMASAMX.MM
X.....XA.A
S.S.S.S.SS
.A.A.A.A.A
..M.M.M.MM
.X.X.XMASX
Take a look at the little Elf's word search. How many times does XMAS appear?
*/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Directions for movement in the grid: {row_offset, col_offset}
const int DIRECTIONS[8][2] = {
    {0, 1},
    {0, -1}, // Horizontal: left-to-right, right-to-left
    {1, 0},
    {-1, 0}, // Vertical: top-to-bottom, bottom-to-top
    {1, 1},
    {-1,
     -1}, // Diagonal (\): top-left to bottom-right, bottom-right to top-left
    {1, -1},
    {-1, 1} // Diagonal (/): top-right to bottom-left, bottom-left to top-right
};

// Function to check if "XMAS" exists starting from (row, col) in a given
// direction
bool findWord(const vector<string> &grid, int row, int col, int dir,
              const string &word) {
  int rows = grid.size();
  int cols = grid[0].size();
  int len = word.length();
  for (int i = 0; i < len; ++i) {
    int r = row + i * DIRECTIONS[dir][0];
    int c = col + i * DIRECTIONS[dir][1];
    if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] != word[i]) {
      return false;
    }
  }
  return true;
}

// Main function to count occurrences of "XMAS"
int countXMAS(const vector<string> &grid) {
  int count = 0;
  const string word = "XMAS";
  int rows = grid.size();
  int cols = grid[0].size();

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      for (int dir = 0; dir < 8; ++dir) {
        if (findWord(grid, row, col, dir, word)) {
          count++;
        }
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
  cout << "Total occurrences of XMAS: " << result << endl;

  return 0;
}
