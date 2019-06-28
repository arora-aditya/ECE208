# Tutorial 3

- Teaches about the use of `klee_assert(0);  //Signal The solution!!` for our benefit
- Teaches how to use `klee` on a more real world problem as compared to the more abstract problems in the first 2 tutorials
- Teaches about the `klee -emit-all-errors` flag
  - Apparently (in most cases) you need only one way to reach an error condition, so KLEE won't show you the other ways to reach the same error state, and will only generate one of the 'err' files

## Other Notes:
- Use `ls klee-last/*.err` to find all the test cases that error-ed out
