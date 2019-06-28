echo "add a klee_assume in main after klee_make_symbolic"
echo "klee_assume(re[SIZE - 1] == '\0');"
echo "can also refer to Regexp_with_assume.c"
clang -I../../klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone Regexp.c
klee Regexp.bc 
