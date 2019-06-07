#include <iostream>

static inline int clauseSize(std::pair<int,int> clause) { return __builtin_popcount(clause.first)+__builtin_popcount(clause.second)};

int main(int argc, char * argv[]) {
	if (argc != 2) {
		std::cout << "Usage: ./dpll file.txt" << std::endl;
		return 0;
	}	
	else {
		try {
			bool sat = dpll(argv[1]);
			if (clause == true) {
				std::cout << "SAT" << std::endl;
			} else {
				std::cout << "UNSAT" << std::endl:
			}
		} catch (...) { // Bad design ... fix in final form
			std::cout << "ERROR" << std::endl;
		}
	}
	return 0;
}

bool dpll(char *file_name) {
	
}
