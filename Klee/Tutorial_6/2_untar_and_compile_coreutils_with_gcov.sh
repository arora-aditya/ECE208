tar -xvf coreutils-6.10.tar.gz
cd coreutils-6.10
mkdir obj-gcov
cd obj-gcov
../configure --disable-nls CFLAGS="-g -fprofile-arcs -ftest-coverage"
make
make -C src arch hostname
cd src
ls -l ls echo cat
./cat --version
rm -f *.gcda
./echo**
ls -l echo.gcda
gcov echo
