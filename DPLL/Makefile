WARNINGS = -Wall -Wextra 
STD = -std=c++14
OUTPUT = -o dpll.o

release:
	g++ dpll.cpp -O3 $(STD) $(OUTPUT)

build:
	g++ dpll.cpp $(STD) $(WARNINGS) $(OUTPUT)

.PHONY: test
test: build
	./test.sh		
.PHONY: test_one
test_one: build
	./dpll.o test/test1.cnf

.PHONY: clean
clean: 
	rm *.o 
