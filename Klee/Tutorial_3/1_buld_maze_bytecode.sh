clang -I../../klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone maze.c
klee -emit-all-errors maze.bc
