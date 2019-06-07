#include <iostream>
#include <fstream>
#include <vector>
#include <set>

bool dpll(char*);
std::pair<std::vector<std::vector<int> >,std::set<int> > parse(char*);
std::vector<int> parseClause(const std::string &);
void addVars(std::set<int> &, const std::vector<int> &);

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
			if (sat == true) {
				std::cout << "SAT" << std::endl;
			} else {
				std::cout << "UNSAT" << std::endl;
			}
		} catch (...) { // Bad design ... fix in final form
			std::cout << "ERROR" << std::endl;
		}
	}
	return 0;
}

bool dpll(char *file_name) {
	auto parsedClause = parse(file_name);
	return true;
}

std::pair<std::vector<std::vector<int> >, std::set<int> > parse(char *file_name) {
	if (file_name == NULL) {
		return {};
	}
	std::ifstream file(file_name);
	if (file.is_open()) {
		std::vector<std::vector<int>> clauses;
		std::set<int> vars;
		std::string l;
		while( getline(file,l)) {
			if ((isNum(l[0]) || l[0] == '-' )) {
				clauses.push_back(parseClause(l));
				addVars(vars,clauses.back());
			}
		}
		return {clauses, vars};
	} else {
		return {};
	}
}

std::vector<int> parseClause(const std::string &s) {
	std::vector<int> clause;
	unsigned int idx, offset;
	while (idx < s.size()) {
		if (! ( isNum(s[idx]) || s[idx] == '-')) {
			idx++;
		} else {
			offset = (s[idx] == '-') ? 1 : 0;
			while (idx+offset < s.size() && isNum(s[idx+offset])) {
				offset += 1;
			}
			if (std::stoi(s.substr(idx,offset-1)) == 0) {
				return clause;
			}
			clause.push_back(std::stoi(s.substr(idx,offset-1)));
		}
	}
	return clause;	
}

void addVars(std::set<int> &vars, const std::vector<int> &clause) {
	for (const int i : clause) {
		vars.insert(abs(i));
	}	
}
