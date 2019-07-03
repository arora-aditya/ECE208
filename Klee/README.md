# Klee

1. Tutorial 1 [here](Tutorial_1/)
2. Tutorial 2 [here](Tutorial_2/)
3. Tutorial 3 [here](Tutorial_3/)
4. Tutorial 4 [here](Tutorial_4/)
5. Tutorial 5 [here](Tutorial_5/)
6. Tutorial 6 [here](Tutorial_6/)

Use Valgrind in combination with Klee to also detect Memory Leaks

## Lecture on Symbolic Execution
Types of Analysis:
1. Static Analysis: You test the code without running it, by just looking at it/scanning it (Every compiler has one Static Analysis tool)
2. Dynamic Analysis: Tests the code by running. eg: Valgrind (does analysis under a specific execution)
3. Symbolic Analysis: Hybrid between the other two
  - Using a virtual machine we can construct all possible conditional paths for a given function
  - We can then use that to draw a conditional tree for them, and apply a SAT solver to generate test inputs for them.

- Types of circuits
  - Combination circuit: They are circuits that do not rely on state, and do not have flip flops in them
  - Sequential circuit: They are circuits that rely on state, and have flip flops

We can convert every program to a a collection of combination commands
