#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

bool dpll(char*);
std::pair<std::vector<std::vector<int> >,std::set<int> > parse(char*);
std::vector<int> parseClause(const std::string &);
void addVars(std::set<int> &, const std::vector<int> &);
bool invalidAssignment(const std::vector<std::vector<int> >&, const std::map<int,bool> &);
bool invalidClause(const std::vector<int> &,const std::map<int,bool>&);
bool invalidVar(const int,const std::map<int,bool> &);

inline bool isNum(char c) {
	return (c >= '0' && c <= '9');
}

template<typename T>
inline T abs(T x) {
	return (x > 0) ? x : (-1)*x; 
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
	std::vector<std::vector<int>> clauses = parsedClause.first;
	std::sort(clauses.begin(), clauses.end(),
			[](std::vector<int> a, std::vector<int> b) -> bool { return a.size() > b.size();}); // Sort by size
	std::set<int> variables = parsedClause.second;
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
				std::sort(	clause.begin(),clause.end(),
						[](int a,int b) -> bool { return a > b;}); // Sort by magnitude
				return clause;
			}
			clause.push_back(std::stoi(s.substr(idx,offset-1)));
		}
	}
	std::sort(	clause.begin(),clause.end(),
			[](int a,int b) -> bool { return a > b;}); 
	return clause;	
}

void addVars(std::set<int> &vars, const std::vector<int> &clause) {
	for (const int i : clause) {
		vars.insert(abs(i));
	}	
}


bool invalidAssignment(const std::vector<std::vector<int>> &cnf, const std::map<int,bool> &assignments) {
	for (const std::vector<int> clause: cnf) {
		if (invalidClause(clause,assignments)) return true;
	}
	return false;
}

bool invalidClause(const std::vector<int> &clause, const std::map<int,bool> &assignments) {
	for (const int i : clause) {
		if (!invalidVar(i,assignments)) return false;
	}
	return true;
}

bool invalidVar(const int i, const std::map<int,bool> &assignments) {
	if (!assignments.count(i)) return false;
	if (assignments.find(i)->second == true)  return false;
	return true;
}
