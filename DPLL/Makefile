WARNINGS = -Wall -Wextra 
STD = -std=c++14
OUTPUT = -o dpll.o

release:
	g++ dpll.cpp -O3 $(STD) $(OUTPUT)

build:
	g++ dpll.cpp $(STD) $(WARNINGS) $(OUTPUT)

.PHONY: clean
clean: 
	rm *.o 
