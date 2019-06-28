clang -I../../klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone get_sign.c
klee get_sign.bc
ktest-tool klee-last/test000001.ktest
ktest-tool klee-last/test000002.ktest
ktest-tool klee-last/test000003.ktest
