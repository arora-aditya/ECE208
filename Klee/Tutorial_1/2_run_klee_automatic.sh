export LD_LIBRARY_PATH=/home/klee/klee_build/lib/:$LD_LIBRARY_PATH
gcc -I ../../klee_src/include/ -L /home/klee/klee_build/lib/ get_sign.c -lkleeRuntest
echo "Expect 0"
KTEST_FILE=klee-last/test000001.ktest ./a.out
echo $?
echo "Expect 1"
KTEST_FILE=klee-last/test000002.ktest ./a.out
echo $?
echo "Expect 255 i.e. unsigned -1"
KTEST_FILE=klee-last/test000003.ktest ./a.out
echo $?
echo "DONE"
