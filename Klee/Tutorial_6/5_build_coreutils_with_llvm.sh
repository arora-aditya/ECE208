export LLVM_COMPILER=clang
cd ..
mkdir obj-llvm
cd obj-llvm
CC=wllvm ../configure --disable-nls CFLAGS="-g -O1 -Xclang -disable-llvm-passes -D__NO_STRING_INLINES  -D_FORTIFY_SOURCE=0 -U__OPTIMIZE__" LDFLAGS="-fuse-ld=gold"
# needed for fucking make step
sudo apt-get install gperf
CC=wllvm make
# echo 'add variables to .bashrc'
make -C src arch hostname
cd src
ls -l ls echo cat
./cat --version
find . -executable -type f | xargs -I '{}' extract-bc '{}'
ls -l ls.bc
klee --libc=uclibc --posix-runtime ./cat.bc --version
