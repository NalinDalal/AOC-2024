/* --- Day 23: LAN Party ---
As The Historians wander around a secure area at Easter Bunny HQ, you come
across posters for a LAN party scheduled for today! Maybe you can find it; you
connect to a nearby datalink port and download a map of the local network (your
puzzle input).

The network map provides a list of every connection between two computers. For
example:

kh-tc
qp-kh
de-cg
ka-co
yn-aq
qp-ub
cg-tb
vc-aq
tb-ka
wh-tc
yn-cg
kh-ub
ta-co
de-co
tc-td
tb-wq
wh-td
ta-ka
td-qp
aq-cg
wq-ub
ub-vc
de-ta
wq-aq
wq-vc
wh-yn
ka-de
kh-ta
co-tc
wh-qp
tb-vc
td-yn
Each line of text in the network map represents a single connection; the line
kh-tc represents a connection between the computer named kh and the computer
named tc. Connections aren't directional; tc-kh would mean exactly the same
thing.

LAN parties typically involve multiplayer games, so maybe you can locate it by
finding groups of connected computers. Start by looking for sets of three
computers where each computer in the set is connected to the other two
computers.

In this example, there are 12 such sets of three inter-connected computers:

aq,cg,yn
aq,vc,wq
co,de,ka
co,de,ta
co,ka,ta
de,ka,ta
kh,qp,ub
qp,td,wh
tb,vc,wq
tc,td,wh
td,wh,yn
ub,vc,wq
If the Chief Historian is here, and he's at the LAN party, it would be best to
know that right away. You're pretty sure his computer's name starts with t, so
consider only sets of three computers where at least one computer's name starts
with t. That narrows the list down to 7 sets of three inter-connected computers:

co,de,ta
co,ka,ta
de,ka,ta
qp,td,wh
tb,vc,wq
tc,td,wh
td,wh,yn
Find all the sets of three inter-connected computers. How many contain at least
one computer with a name that starts with t?

Your puzzle answer was 1358.

--- Part Two ---
There are still way too many results to go through them all. You'll have to find
the LAN party another way and go there yourself.

Since it doesn't seem like any employees are around, you figure they must all be
at the LAN party. If that's true, the LAN party will be the largest set of
computers that are all connected to each other. That is, for each computer at
the LAN party, that computer will have a connection to every other computer at
the LAN party.

In the above example, the largest set of computers that are all connected to
each other is made up of co, de, ka, and ta. Each computer in this set has a
connection to every other computer in the set:

ka-co
ta-co
de-co
ta-ka
de-ta
ka-de
The LAN party posters say that the password to get into the LAN party is the
name of every computer at the LAN party, sorted alphabetically, then joined
together with commas. (The people running the LAN party are clearly a bunch of
nerds.) In this example, the password would be co,de,ka,ta.

What is the password to get into the LAN party?

Your puzzle answer was cl,ei,fd,hc,ib,kq,kv,ky,rv,vf,wk,yx,zf.
*/
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

// Function to check if a name starts with 't'
inline bool startsWithT(const string &name) {
  return !name.empty() && name.front() == 't';
}

// Function to find triangles in the graph
set<tuple<string, string, string>>
findTriangles(const unordered_map<string, unordered_set<string>> &graph) {
  set<tuple<string, string, string>> triangles;

  for (const auto &[node, neighbors] : graph) {
    for (auto it1 = neighbors.begin(); it1 != neighbors.end(); ++it1) {
      for (auto it2 = next(it1); it2 != neighbors.end(); ++it2) {
        if (graph.at(*it1).count(*it2)) {
          vector<string> nodes = {node, *it1, *it2};
          sort(nodes.begin(), nodes.end());
          triangles.emplace(nodes[0], nodes[1], nodes[2]);
        }
      }
    }
  }

  return triangles;
}

// Function to find the largest clique in the graph
vector<string>
findLargestClique(const unordered_map<string, unordered_set<string>> &graph) {
  vector<string> largestClique;

  // Iterate over all subsets of the graph
  for (const auto &[node, neighbors] : graph) {
    vector<string> clique = {node};
    for (const auto &neighbor : neighbors) {
      if (all_of(clique.begin(), clique.end(), [&](const string &member) {
            return graph.at(member).count(neighbor);
          })) {
        clique.push_back(neighbor);
      }
    }
    if (clique.size() > largestClique.size()) {
      largestClique = move(clique);
    }
  }

  sort(largestClique.begin(), largestClique.end());
  return largestClique;
}

int main() {
  ifstream input("a.in");
  if (!input) {
    cerr << "Failed to open the input file." << endl;
    return EXIT_FAILURE;
  }

  unordered_map<string, unordered_set<string>> graph;
  string line;

  // Build the graph from input
  while (getline(input, line)) {
    auto dashPos = line.find('-');
    string node1 = line.substr(0, dashPos);
    string node2 = line.substr(dashPos + 1);

    graph[node1].insert(node2);
    graph[node2].insert(node1);
  }

  input.close();

  // Find all triangles
  auto triangles = findTriangles(graph);

  // Count triangles with at least one node starting with 't'
  int count =
      count_if(triangles.begin(), triangles.end(), [](const auto &triangle) {
        return startsWithT(get<0>(triangle)) || startsWithT(get<1>(triangle)) ||
               startsWithT(get<2>(triangle));
      });

  cout << "Number of triangles with at least one node starting with 't': "
       << count << endl;

  // Find the largest clique
  auto largestClique = findLargestClique(graph);

  cout << "Password to access the LAN party: ";
  for (size_t i = 0; i < largestClique.size(); ++i) {
    cout << largestClique[i];
    if (i < largestClique.size() - 1)
      cout << ",";
  }
  cout << endl;

  return EXIT_SUCCESS;
}
