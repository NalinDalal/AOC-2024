"""
--- Part Two ---
Now that you know what the best paths look like, you can figure out the best spot to sit.

Every non-wall tile (S, ., or E) is equipped with places to sit along the edges of the tile. While determining which of these tiles would be the best spot to sit depends on a whole bunch of factors (how comfortable the seats are, how far away the bathrooms are, whether there's a pillar blocking your view, etc.), the most important factor is whether the tile is on one of the best paths through the maze. If you sit somewhere else, you'd miss all the action!

So, you'll need to determine which tiles are part of any best path through the maze, including the S and E tiles.

In the first example, there are 45 tiles (marked O) that are part of at least one of the various best paths through the maze:

###############
#.......#....O#
#.#.###.#.###O#
#.....#.#...#O#
#.###.#####.#O#
#.#.#.......#O#
#.#.#####.###O#
#..OOOOOOOOO#O#
###O#O#####O#O#
#OOO#O....#O#O#
#O#O#O###.#O#O#
#OOOOO#...#O#O#
#O###.#.#.#O#O#
#O..#.....#OOO#
###############
In the second example, there are 64 tiles that are part of at least one of the best paths:

#################
#...#...#...#..O#
#.#.#.#.#.#.#.#O#
#.#.#.#...#...#O#
#.#.#.#.###.#.#O#
#OOO#.#.#.....#O#
#O#O#.#.#.#####O#
#O#O..#.#.#OOOOO#
#O#O#####.#O###O#
#O#O#..OOOOO#OOO#
#O#O###O#####O###
#O#O#OOO#..OOO#.#
#O#O#O#####O###.#
#O#O#OOOOOOO..#.#
#O#O#O#########.#
#O#OOO..........#
#################
Analyze your map further. How many tiles are part of at least one of the best paths through the maze?


"""

# Python heap implementation
from heapq import heappush, heappop
from collections import defaultdict

with open("./day_16.in") as fin:
    grid = fin.read().strip().split("\n")

n = len(grid)
for i in range(n):
    for j in range(n):
        if grid[i][j] == "S":
            start = (i, j)
        elif grid[i][j] == "E":
            end = (i, j)

dd = [[0, 1], [1, 0], [0, -1], [-1, 0]]

# Dijkstra's
q = [(0, 0, *start, 0, *start)]
cost = {}
deps = defaultdict(list)
while len(q) > 0:
    top = heappop(q)
    c, d, i, j, pd, pi, pj = top
    if (d, i, j) in cost:
        if cost[(d, i, j)] == c:
            deps[(d, i, j)].append((pd, pi, pj))
        continue

    never_seen_pos = True
    for newd in range(4):
        if (newd, i, j) in cost:
            never_seen_pos = True
            break
    if never_seen_pos:
        deps[(d, i, j)].append((pd, pi, pj))

    cost[(d, i, j)] = c

    if grid[i][j] == "#":
        continue

    if grid[i][j] == "E":
        end_dir = d
        print(c)
        break

    ii = i + dd[d][0]
    jj = j + dd[d][1]

    for nbr in [
        (c + 1, d, ii, jj, d, i, j),
        (c + 1000, (d + 1) % 4, i, j, d, i, j),
        (c + 1000, (d + 3) % 4, i, j, d, i, j),
    ]:
        heappush(q, nbr)

# Go back through deps
stack = [(end_dir, *end)]
seen = set()
seen_pos = set()
while len(stack) > 0:
    top = stack.pop()
    if top in seen:
        continue
    seen.add(top)
    seen_pos.add(top[1:])

    for nbr in deps[top]:
        stack.append(nbr)

print(len(seen_pos))

# for i in range(n):
#     for j in range(n):
#         if (i, j) in seen:
#             print("OO", end="")
#         else:
#             print("..", end="")
#     print()
