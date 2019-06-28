echo "int main(int argn, char** argv) { return 0; }" > test.c
clang -emit-llvm -g -c test.c -o test.bc
klee --libc=uclibc --posix-runtime test.bc
