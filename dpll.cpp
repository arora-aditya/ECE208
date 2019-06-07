#include <iostream>
#include <fstream>
#include <vector>
#include <set>

inline bool isNum(char c) {
	return (c >= '0' && c <= '9');
}

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
	int err = 0;
	auto parsedClause = parse(file_name,err);
}

std::pair<std::vector<std::vector<int>, std::set<int>> parse(char *file_name, int &err) {
	if (file_name == NULL) {
		err = 1;
		return {};
	}
	ifstream file(file_name);
	if (file.is_open()) {
		std::vector<std::vector<int>> clauses;
		std::set<int> vars;
		std::string l;
		while( getline(file,l) {
			if ((isChar(l[0]) || l[0] == '-' ) {
				clauses.push_back(parseClause(l));
				addVars(vars,clauses.back());
			}
		}
		return {clauses, vars}
	} else {
i		err = 1;
		return {};
	}
}

std::vector<int> parseClause(const std::string &s) {
	std::vector<int> clause;
	int idx, offset;
	while (idx < s.size()) {
		if (! ( isChar(s[idx] || s[idx] == '-')) {
			idx++;
		} else {
			offset = (s[idx] == '-') ? 1 : 0;
			while (idx+offset < s.size() && isChar(s[idx+offset])) {
				offset += 1;
			}
			clause.push_back(stoi(s.substr(idx,offset-1)));
		}
	}
	return clause;	
}

void addVars(std::set &vars, const std::vector<int> &clause) {
	for (auto int &&i : clause) {
		vars.insert(abs(i));
	}	
}
