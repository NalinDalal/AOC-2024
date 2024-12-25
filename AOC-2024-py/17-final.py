class ThreeBitComputer:
    def __init__(self, program, reg_a=0, reg_b=0, reg_c=0, max_steps=1000000):
        self.program = program
        self.reg_a = reg_a
        self.reg_b = reg_b
        self.reg_c = reg_c
        self.instruction_pointer = 0
        self.output = []
        self.max_steps = max_steps
        self.steps = 0

    def get_combo_value(self, operand):
        if operand <= 3:
            return operand
        elif operand == 4:
            return self.reg_a
        elif operand == 5:
            return self.reg_b
        elif operand == 6:
            return self.reg_c
        else:
            raise ValueError(f"Invalid combo operand: {operand}")

    def execute_instruction(self):
        if self.instruction_pointer >= len(self.program):
            return False

        self.steps += 1
        if self.steps > self.max_steps:
            return False

        opcode = self.program[self.instruction_pointer]
        operand = self.program[self.instruction_pointer + 1]

        if opcode == 0:  # adv
            self.reg_a //= 2 ** self.get_combo_value(operand)
            self.instruction_pointer += 2
        elif opcode == 1:  # bxl
            self.reg_b ^= operand
            self.instruction_pointer += 2
        elif opcode == 2:  # bst
            self.reg_b = self.get_combo_value(operand) % 8
            self.instruction_pointer += 2
        elif opcode == 3:  # jnz
            if self.reg_a != 0:
                self.instruction_pointer = operand
            else:
                self.instruction_pointer += 2
        elif opcode == 4:  # bxc
            self.reg_b ^= self.reg_c
            self.instruction_pointer += 2
        elif opcode == 5:  # out
            value = self.get_combo_value(operand) % 8
            self.output.append(str(value))
            self.instruction_pointer += 2
        elif opcode == 6:  # bdv
            self.reg_b = self.reg_a // (2 ** self.get_combo_value(operand))
            self.instruction_pointer += 2
        elif opcode == 7:  # cdv
            self.reg_c = self.reg_a // (2 ** self.get_combo_value(operand))
            self.instruction_pointer += 2
        else:
            raise ValueError(f"Invalid opcode: {opcode}")

        return True

    def run(self):
        while self.execute_instruction():
            pass
        return ",".join(self.output)


def parse_input(filename):
    with open(filename, "r") as f:
        lines = f.readlines()

    reg_a = reg_b = reg_c = 0
    program = []

    for line in lines:
        if line.startswith("Register A:"):
            reg_a = int(line.split(":")[1].strip())
        elif line.startswith("Register B:"):
            reg_b = int(line.split(":")[1].strip())
        elif line.startswith("Register C:"):
            reg_c = int(line.split(":")[1].strip())
        elif line.startswith("Program:"):
            program = [int(x) for x in line.split(":")[1].strip().split(",")]

    return program, reg_a, reg_b, reg_c


def part1(program, reg_a, reg_b, reg_c):
    computer = ThreeBitComputer(program, reg_a, reg_b, reg_c)
    return computer.run()


def part2(program, start=1, step=1):
    target = ",".join(str(x) for x in program)
    print(f"Looking for output matching: {target}")

    # Try larger increments to find the rough area faster
    a = start
    while True:
        if a % 10000 == 0:
            print(f"Trying A = {a}")
            print(f"Last output: {ThreeBitComputer(program, a, 0, 0).run()}")

        computer = ThreeBitComputer(program, a, 0, 0)
        output = computer.run()
        if output == target:
            return a
        if computer.steps >= computer.max_steps:
            print(
                f"A = {a} caused potential infinite loop after {computer.steps} steps"
            )
            print(f"Output so far: {output}")

        a += step


def main():
    # Read input from 17.in
    program, reg_a, reg_b, reg_c = parse_input("17.in")

    # Part 1
    print("Part 1:", part1(program, reg_a, reg_b, reg_c))

    # Part 2 with better debugging
    print("\nPart 2:")
    print("Program:", program)
    result = part2(program)
    print("Result:", result)


if __name__ == "__main__":
    main()
