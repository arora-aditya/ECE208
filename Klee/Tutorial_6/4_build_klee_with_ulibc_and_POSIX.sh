sudo apt-get install build-essential curl libcap-dev git cmake libncurses5-dev python-minimal python-pip unzip libtcmalloc-minimal4 libgoogle-perftools-dev libsqlite3-dev doxygen
pip3 install tabulate
sudo apt-get install clang-3.8 llvm-3.8 llvm-3.8-dev llvm-3.8-tools
sudo apt-get install cmake bison flex libboost-all-dev python perl minisat
git clone https://github.com/stp/stp
cd stp
mkdir build
cd build
cmake ..
make
sudo make install
cd ..
cd ..
git clone https://github.com/klee/klee-uclibc.git
cd klee-uclibc
# for the next command you might have to alias LLVM to point the right location
# if it fails, lookup ./configure --help
./configure --make-llvm-lib
make -j2
cd ..
git clone https://github.com/klee/klee.git
mkdir klee_build_dir
cd klee_build_dir
cmake \
  -DENABLE_SOLVER_STP=ON \
  -DENABLE_POSIX_RUNTIME=ON \
  -DENABLE_KLEE_UCLIBC=ON \
  -DKLEE_UCLIBC_PATH=../klee-uclibc/ \
  -DGTEST_SRC_DIR=<GTEST_SOURCE_DIR> \
  -DENABLE_SYSTEM_TESTS=ON \
  -DENABLE_UNIT_TESTS=OF \
  ../klee
make
make check
