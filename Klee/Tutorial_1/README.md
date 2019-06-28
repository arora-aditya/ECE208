# Tutorial 1

- Teaches how to compile to generate the llvm byte code for klee to use:
```sh
clang -I../../klee_src/include -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone get_sign.c
```
- Teaches how to use klee to generate test cases:
`klee get_sign.bc`
- Run a test case manually
```sh
ktest-tool klee-last/test000001.ktest
```
- Run test case automatically
```sh
export LD_LIBRARY_PATH=/home/klee/klee_build/lib/:$LD_LIBRARY_PATH
gcc -I ../../klee_src/include/ -L /home/klee/klee_build/lib/ get_sign.c -lkleeRuntest
echo "Expect 255 i.e. unsigned -1"
KTEST_FILE=klee-last/test000003.ktest ./a.out
echo $?
```

## Other Notes
- `klee-last` is symlinked to the last created klee-test folder
- Use `cat > filename` to write to a file in the absence of vim. Leave an empty line at the end, and use Ctrl+D to finish writing the file
