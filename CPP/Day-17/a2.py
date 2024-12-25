"""--- Part Two ---
Digging deeper in the device's manual, you discover the problem: this program is supposed to output another copy of the program! Unfortunately, the value in register A seems to have been corrupted. You'll need to find a new value to which you can initialize register A so that the program's output instructions produce an exact copy of the program itself.

For example:

Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0
This program outputs a copy of itself if register A is instead initialized to 117440. (The original initial value of register A, 2024, is ignored.)

What is the lowest positive initial value for register A that causes the program to output a copy of itself?

"""
# WARNING: only works with my specific input
# May modify in the future to work on general inputs

with open("./17.in") as fin:
    lines = fin.read().strip().split("\n")
    A, B, C = [int(lines[i].split(" ")[2]) for i in range(3)]
    program = list(map(int, lines[4].split(" ")[1].split(",")))

valid = {}
tree = {}
cur = 0

for cur in range(len(program)):
    # Stores set of possible 10 bits at given position
    valid[cur] = set()
    out = program[cur]

    def get_compatible(cur, bits, prev):
        for prev_bits in valid[prev]:
            overlap = 10 - (cur - prev) * 3
            prefix = prev_bits >> (10 - overlap)
            if prefix == bits % (1 << overlap):
                yield (prev, prev_bits)

    for bits in range(2**10):
        tree[(cur, bits)] = set()
        R = bits % 8

        # This is the section that's specific to my input
        if (R ^ (bits >> (R ^ 6)) ^ 2) % 8 == out:
            # Vet this valid section with existing valids
            for prev in range(cur - 3, cur):
                if prev < 0:
                    continue
                tree[(cur, bits)].update(get_compatible(cur, bits, prev))

            if cur == 0 or len(tree[(cur, bits)]) > 0:
                valid[cur].add(bits)


# Follow the tree
possible_A = []
stack = []

for bits in valid[len(program) - 1]:
    if bits >= 256:
        continue

    stack.append((0, len(program) - 1, bits))

print(stack)
ans = []

while len(stack) > 0:
    A, cur, bits = stack.pop()
    A = (A << 3) ^ (bits % 8)

    if cur == 0:
        ans.append(A)

    for child in tree[(cur, bits)]:
        if child[0] != cur - 1:
            continue
        stack.append((A, *child))

print(sorted(ans)[0])
