WARNINGS = -Wall -Wextra
STD = -std=c++14
OUTPUT = dpll.o

release:
	g++ dpll.cc -O3 $(STD) -o $(OUTPUT)

build:
	g++ dpll.cc $(STD) $(WARNINGS) -o $(OUTPUT)

.PHONY: test
test: build
	./test.sh		
.PHONY: test_one
test_one: build
	./$(OUTPUT) test/test1.cnf

.PHONY: clean
clean: 
	rm *.o 
