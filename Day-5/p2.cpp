#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Function to check if an update is valid
bool is_valid_update(const vector<int> &update,
                     const unordered_map<int, unordered_set<int>> &rules) {
  unordered_map<int, int> position;
  for (int i = 0; i < update.size(); ++i) {
    position[update[i]] = i;
  }
  for (const auto &pair : rules) {
    int x = pair.first;
    const auto &y_set = pair.second;
    if (position.find(x) == position.end())
      continue;
    for (int y : y_set) {
      if (position.find(y) == position.end())
        continue;
      if (position[x] >= position[y]) {
        return false;
      }
    }
  }
  return true;
}

// Function to reorder an update based on the rules
vector<int>
reorder_update(const vector<int> &update,
               const unordered_map<int, unordered_set<int>> &rules) {
  unordered_map<int, int> indegree;
  unordered_map<int, vector<int>> graph;
  unordered_set<int> unique_pages(update.begin(), update.end());

  // Build the graph and compute in-degrees
  for (int page : unique_pages) {
    indegree[page] = 0;
  }
  for (const auto &pair : rules) {
    int x = pair.first;
    for (int y : pair.second) {
      if (unique_pages.count(x) && unique_pages.count(y)) {
        graph[x].push_back(y);
        indegree[y]++;
      }
    }
  }

  // Topological sort using Kahn's algorithm
  queue<int> q;
  for (int page : unique_pages) {
    if (indegree[page] == 0) {
      q.push(page);
    }
  }

  vector<int> sorted_update;
  while (!q.empty()) {
    int current = q.front();
    q.pop();
    sorted_update.push_back(current);

    for (int neighbor : graph[current]) {
      indegree[neighbor]--;
      if (indegree[neighbor] == 0) {
        q.push(neighbor);
      }
    }
  }

  return sorted_update;
}

int main() {
  ifstream fin("p1.txt");
  if (!fin) {
    cerr << "Error: Unable to open input file." << endl;
    return 1;
  }

  unordered_map<int, unordered_set<int>> rules;
  string line;

  // Read the rules
  while (getline(fin, line) && !line.empty()) {
    stringstream ss(line);
    int x, y;
    char pipe;
    ss >> x >> pipe >> y;
    rules[x].insert(y);
  }

  // Read the updates
  vector<vector<int>> updates;
  while (getline(fin, line)) {
    stringstream ss(line);
    vector<int> update;
    int page;
    while (ss >> page) {
      if (ss.peek() == ',')
        ss.ignore();
      update.push_back(page);
    }
    updates.push_back(update);
  }

  fin.close();

  int sum_of_fixed_middle = 0;

  for (const auto &update : updates) {
    if (!is_valid_update(update, rules)) {
      // Invalid update: Reorder and calculate middle page
      vector<int> reordered = reorder_update(update, rules);
      int middle_index = reordered.size() / 2;
      sum_of_fixed_middle += reordered[middle_index];
    }
  }

  cout << "Sum of middle pages of fixed updates: " << sum_of_fixed_middle
       << endl;

  return 0;
}
