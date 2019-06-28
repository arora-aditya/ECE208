# Tutorial 3

- Teaches about how to add timeouts to the execution of `klee`
  - `-max-time=seconds`: Halt execution after the given number of seconds.
  - `-max-forks=N`: Stop forking after `N` symbolic branches, and run the remaining paths to termination.
  - `-max-memory=N`: Try to limit memory consumption to `N` megabytes
- Teaches about the Klee Error reports, additional information about the error is stored in a file `testN.TYPE.err`, where `N` is the test case number, and `TYPE` identifies the kind of error:
  - `ptr`: Stores or loads of invalid memory locations.
  - `free`: Double or invalid free().
  - `abort`: The program called abort().
  - `assert`: An assertion failed.
  - `div`: A division or modulus by zero was detected.
  - `user`: There is a problem with the input (invalid klee intrinsic calls) or the way KLEE is being used.
  - `exec`: There was a problem which prevented KLEE from executing the program; for example an unknown instruction, a call to an invalid function pointer, or inline assembly.
  - `model`: KLEE was unable to keep full precision and is only exploring parts of the program state. For example, symbolic sizes to malloc are not currently supported, in such cases KLEE will concretize the argument.
- Teaches the concept that: Making a buffer symbolic just initializes the contents to refer to symbolic variables, but that we are still free to modify the memory as we wish.
- Teaches to use `klee-assume` which allows us to extend the assumptions that we make in code about the input for `klee` to use
  - By explicitly declaring constraints in `klee_assume`, it will force test cases to have the condition imposed on them.  Eg: `klee_assume(re[SIZE - 1] == '\0');` will force test cases to have the '\0' in them.
  - `klee_assume` can be used to encode more complicated constraints. For example, we could use `klee_assume(re[0] != '^')` to cause KLEE to only explore states where the first byte is not `'^'`.
  - **NOTE**: One important caveat when using `klee_assume` with multiple conditions; remember that boolean conditionals like `'&&'` and `'||'` may be compiled into code which branches before computing the result of the expression. In such situations KLEE will branch the process _before_ it reaches the call to `klee_assume`, which may result in exploring unnecessary additional states. For this reason it is good to use as simple expressions as possible to `klee_assume` (for example splitting a single call into multiple ones), and to use the `'&'` and `'|'` operators instead of the short-circuiting ones.
