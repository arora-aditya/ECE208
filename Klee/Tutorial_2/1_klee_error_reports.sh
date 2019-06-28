clang -I../../klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone Regexp.c
echo "find the .err files in klee-out-N"
echo "run the file as 'ktest-tool klee-last/test000007.ktest' to get description of the test case"
