#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>

#define DEBUG_DPLL false
#define DEBUG_PARSE false
bool dpll(char*);
bool dpllInner(const std::vector<std::vector<int>> &,std::set<int>,std::map<int,bool>);
std::pair<std::vector<std::vector<int> >,std::set<int> > parse(char*);
std::vector<int> parseClause(const std::string &);
void addVars(std::set<int> &, const std::vector<int> &);
bool badClause(const std::vector<int> &);
bool invalidAssignment(const std::vector<std::vector<int> >&, const std::map<int,bool> &);
bool validClause(const std::vector<int> &,const std::map<int,bool>&);
bool validVar(const int,const std::map<int,bool> &);
void printAssignments(const std::map<int,bool> &);
void dumpVector(const std::vector<int> &);


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
		} catch (int e) {
			switch(e) {
				case 1: std::cout << "Error: Filename Pointer is invalid" << std::endl;
					break;
				case 2: std::cout << "Error: Couldn't open the file" << std::endl;
					break;
				case 3: std::cout << "Error: Invalid line in file" << std::endl;
					break;
				case 4: std::cout << "!!UNSAT" << std::endl;	// This is tricky, but basically if any one clause
					break;					// can't be sat, then it can just jump to unsat.
				default: std::cout << "Error: no " << e << std::endl;
			}
			return 1;
		} /*catch (...) {	
			std::cout << "ERROR" << std::endl;
			return 1;
		}*/
	}
	return 0;
}

bool dpll(char *file_name) {
	auto parsedClause = parse(file_name);
	std::vector<std::vector<int>> clauses = parsedClause.first;
	if (clauses.empty()) return true;
	std::sort(clauses.begin(), clauses.end(),
			[](std::vector<int> a, std::vector<int> b) -> bool { return a.size() > b.size();}); // Sort by size
	std::set<int> variables = parsedClause.second;
	if (DEBUG_PARSE) std::cout << "Handling : " << clauses.size() << "clauses with " << variables.size() << " variables\n";
	std::map<int,bool> dict;
	return dpllInner(clauses,variables,dict);
}

bool dpllInner(const std::vector<std::vector<int>> &formula, std::set<int> unassigned, std::map<int,bool> assigned) {
	//std::cout << "Dump In vec";
	//dumpVector(formula[0]);
	if (unassigned.empty()) {
		bool out = !invalidAssignment(formula, assigned);
		if (out && DEBUG_DPLL) {
			printAssignments(assigned);
		}
		return out;
	}
	else if (invalidAssignment(formula,assigned)) return false;
	int var = (*unassigned.begin());
	unassigned.erase(var);
	//if (DEBUG_DPLL) std::cout << "Took var out of clause: " << var << "\n";
	assigned[var] = true;
	if (dpllInner(formula,unassigned,assigned)) return true;
	assigned[var] = false;
	if (dpllInner(formula,unassigned,assigned)) return true;
	return false;
}

std::pair<std::vector<std::vector<int>>, std::set<int>> parse(char *file_name) {
	if (file_name == NULL) { // No file name error - should not happen
		throw 1;
		return {};
	}
	std::ifstream file(file_name); // Open file from name
	if (file.is_open()) {
		std::vector<std::vector<int>> clauses;
		std::set<int> vars;
		std::string l;
		while( getline(file,l)) {
			if (l[0] == 'c') {
				// Comment Line, do nothing
			} else if (l[0] == 'p') {
				// Description, but our method of parsing does not require us to know this in advance
			} else if ((isNum(l[0]) || l[0] == '-' )) { // parse each clause
				clauses.push_back(parseClause(l));
				if (clauses.back().empty()) { 
					clauses.pop_back();
				} else {
					addVars(vars,clauses.back()); // add it to the set for BCP assignment
				}
			} else {
				if (DEBUG_PARSE) std::cout << "Invalid line from the start";
				throw 3;
			}
		}
		if (DEBUG_PARSE) std::cout << "Returning from scope with size: " << clauses.size() << std::endl;
		return {clauses, vars};
	} else { // File can't open error
		throw 2;
		return {};
	}
}

std::vector<int> parseClause(const std::string &s) {
	if (s.back() != '0') throw 3;
	std::vector<int> clause;
	unsigned int idx, offset;
	idx = 0;
	while (idx < s.size()) {
		while (! ( isNum(s[idx]) || s[idx] == '-')) { // parse whitespace
			if (s[idx] != ' ') {
				if (DEBUG_PARSE) std::cout << "Invalid whitespace while moving iterator";
				throw 3;
			}
			idx++;
		}
		offset = 0;
		if (s[idx] == '-') {	
			if (!isNum(s[idx+1])) {	
				if (DEBUG_PARSE) std::cout << "No characters after a negative sign";
				throw 3; // Negative with no digit following it
			}
			offset += 1;
		}
		while (idx+offset < s.size() && isNum(s[idx+offset])) {
			offset += 1;
		}
		if (s[idx+offset] != ' ' && idx+offset < s.size())  {
			if (DEBUG_PARSE) std::cout << "Invalid whitespace when adjusting offset";
			throw 3; // Non space seperator
		}
		if (DEBUG_PARSE) std::cout << "Trying to convert to int: " << s.substr(idx,offset) << "\n";
		if (std::stoi(s.substr(idx,offset)) == 0) {
			std::sort(	clause.begin(),clause.end(),
					[](int a,int b) -> bool { return a > b;}); // Sort by magnitude
			if (badClause(clause)) return{};
			return clause;
		}
		clause.push_back(std::stoi(s.substr(idx,offset)));
		idx = idx+offset+1;
	}
	std::sort(	clause.begin(),clause.end(),
			[](int a,int b) -> bool { return a > b;}); 
	if (badClause(clause)) return {};
	return clause;	
}

void addVars(std::set<int> &vars, const std::vector<int> &clause) {
	for (const int i : clause) {
		if (DEBUG_PARSE) std::cout << "Adding : " << abs(i) << "\n";
		vars.insert(abs(i));
	}
	return;	
}

bool badClause(const std::vector<int> &clause) { // This checks for contradictions in each local clause
	if (DEBUG_PARSE) {
		std::cout << "Parsing clause : ";
		for (const int i : clause) {
			std::cout << i << ",";
		}
		std::cout << "\n";
	}
	std::set<int> absNums;
	std::set<int> nums;
	for (const int i : clause) {
		if (absNums.count(abs(i))) {
			if (nums.count(-1*i)) return true;
		} else {
			absNums.insert(abs(i));
			nums.insert(i);
		}
	}
	return false;
}

bool invalidAssignment(const std::vector<std::vector<int>> &cnf, const std::map<int,bool> &assignments) {
	for (const std::vector<int> clause: cnf) {
		if (!validClause(clause,assignments)) return true; // If any clause is invalid, the whole formula is
	}
	return false;
}

bool validClause(const std::vector<int> &clause, const std::map<int,bool> &assignments) {
	for (const int i : clause) {
		if (validVar(i,assignments)) return true; // if one assignment is valid, the whole clause is
	}
	return false;
}

bool validVar(const int i, const std::map<int,bool> &assignments) {
	if (!assignments.count(abs(i))) return true; // unassigned can still be tru
	bool assn = assignments.find(abs(i))->second;
	return (( i > 0 && assn == true) || (i < 0 && assn == false));
}

void printAssignments(const std::map<int,bool> &dict) {
	std::cout << "Assignments are :\n";
	for (auto it = dict.begin(); it != dict.end(); it++) {
		std::cout << it->first << " -> " << it->second << ",";
	}
	std::cout << "\n";
	return;
}

void dumpVector(const std::vector<int> &vec) {
	std::cout << "Vec: ";
	for (const int i : vec) {
		std::cout << i << " ";
	}
	std::cout << "\n";
}	
