#include "dpll.hh"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

#define DEBUG_DPLL false
#define DEBUG_PARSE false
#define DO_UCP true
#define VEC_COMP [](std::vector<int> a, std::vector<int> b) -> bool { return a.size() < b.size(); }
#define ABS_INT_COMP [](int a, int b) -> bool { return abs(a) > abs(b); }

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
				case 4: std::cout << "Error: File missing header" << std::endl;
					break;
				case 5: std::cout << "UNSAT" << std::endl;
					break;
				case 6: std::cout << "SAT" << std::endl;
					break;
				default: std::cout << "Error: no " << e << std::endl;
			}
			return 1;
		} catch (...) {	
			std::cout << "ERROR" << std::endl;
			return 1;
		}
	}
	return 0;
}

bool dpll(char *file_path) {
	auto parsedClause = parse(file_path);
	auto clauses = parsedClause.first;
	auto variables = parsedClause.second;
	if (clauses.empty()) {
		if (DEBUG_DPLL) { std::cout << "Clauses empty, returning early\n"; }
		return false;
	}
	if (hasEmptyClause(clauses)) {
		if (DEBUG_DPLL) { std::cout << "Clauses empty, returning early\n"; }
		return false;
	}
	std::sort(clauses.begin(), clauses.end(),VEC_COMP); // Sort by size
	if (DEBUG_PARSE) { std::cout << "Handling : " << clauses.size() << "clauses with " << variables.size() << " variables\n"; }
	std::map<int,bool> assignments;
	if (!propogateLogic(assignments,clauses)) {
		if (DEBUG_DPLL) { std::cout << "Logic propogation failed, returning early\n"; }
		return false;
	}
	return dpllInner(clauses,variables,assignments);
}

bool dpllOpt(char *file_path, std::map<int,bool> *opt_assignment) {
	auto parsedClause = parse(file_path);
	auto clauses = parsedClause.first;
	auto variables = parsedClause.second;
	if (clauses.empty()) return true;
	std::sort(clauses.begin(), clauses.end(),VEC_COMP); // Sort by size
	if (DEBUG_PARSE) { std::cout << "Handling : " << clauses.size() << "clauses with " << variables.size() << " variables\n"; }
	std::map<int,bool> assignments;
	if (!propogateLogic(assignments,clauses)) return false;
	return dpllInnerOpt(clauses,variables,assignments,opt_assignment);
}

bool dpllInner(std::vector<std::vector<int>> formula, std::set<int> unassigned, std::map<int,bool> assigned) {
	if (DEBUG_DPLL) { printAssignments(assigned); }
	if (invalidAssignment(formula,assigned)) return false; // If conflict, return false

	int ret = doBCP(formula,assigned,unassigned); // even this signature is off - fix after demo
	if (ret == BCP_UNSAT) {
		if (DEBUG_DPLL) { std::cout << "Failed on BCP"; }
		return false;
	}
	else if (ret == BCP_SAT) {
		if (DEBUG_DPLL) { std::cout << "Succeeded from BCP"; }
		return true;
	}
	assert(ret == BCP_OK && "Expecting BCP_OK");
	if (invalidAssignment(formula,assigned)) {
		return false;
	} else if (unassigned.empty()) { // If there's no variables left to assign, check for conflicts
		return true;
	}
	
	int var = (*unassigned.begin());
	unassigned.erase(var); // remove from set once assigning

	assigned[var] = true;
	if (dpllInner(formula,unassigned,assigned)) return true;

	assigned[var] = false;
	if (dpllInner(formula,unassigned,assigned)) return true;

	return false; // if neither option can satisfy, return false
}

bool dpllInnerOpt(const std::vector<std::vector<int>> &formula, std::set<int> unassigned, std::map<int,bool> assigned, std::map<int,bool> *opt_assignment) {
	if (invalidAssignment(formula,assigned)) return false; // If conflict, return false
	else if (unassigned.empty()) { // If there's no variables left to assign, check for conflicts
		if (opt_assignment != NULL) { *opt_assignment = std::move(assigned); }
		return true;
	}
	
	int var = (*unassigned.begin());
	unassigned.erase(var); // remove from set once assigning

	assigned[var] = true;
	if (dpllInnerOpt(formula,unassigned,assigned,opt_assignment)) return true;

	assigned[var] = false;
	if (dpllInnerOpt(formula,unassigned,assigned,opt_assignment)) return true;

	return false; // if neither option can satisfy, return false
}

int doBCP(std::vector<std::vector<int>> &formula, std::map<int,bool> assigned, std::set<int> unassigned) {
	/*
	 * Note to future self: This actual implementation does nothing more than the bare minimum
	 * The spec of implementing this could be done proplerly with a topological sort style implementation
	 * Which seems like it'd be better. This is designed to be lazy
 	 */
	int flag = 0;

	/*
	 * This block will simplify clauses and return on a fail
 	 */

	auto formula_it = formula.begin();
	while (formula_it != formula.end() && !formula.empty()) {
		auto vec_it = formula_it->begin();
		while (vec_it != formula_it->end() && !formula_it->empty()) {
			int val = *vec_it; // Just to make it cleaner
			if (assigned.count(abs(val))) {
				if ((val > 0 && assigned[val] == true) || (val < 0 && assigned[-val] == false)) { 
					formula.erase(formula_it);
					if (formula.empty()) return BCP_SAT;
					formula_it = formula.begin();
					flag = 1;
					break;
				} else {
					formula_it->erase(vec_it);
					if (formula_it->empty()) return BCP_UNSAT;
					vec_it = formula_it->begin();
				}
			} else {
				vec_it++;
			}	
		}
		if (flag == 1) {
			flag = 0;
		} else {
			formula_it++;
		}
	}
	std::sort(formula.begin(),formula.end(),VEC_COMP);
	for (auto it = formula.begin(); it != formula.end() && it->size() < 2; it++) {
		assert(!it->empty() && "Error, we didn't remove an empty vector");
		assert(!assigned.count(abs(it->front())) && "Error, we didn't remove an assigned variable");
		int val = it->front();
		assert(val != 0 && "Error, 0 is not valid variable");
		if (val > 0) {
			assigned[val] = true;
			unassigned.erase(val);
		} else {
			assigned[-val] = false;
			unassigned.erase(-val);
		}
	}
	return BCP_OK;
}

std::pair<std::vector<std::vector<int>>, std::set<int>> parse(char *file_name) {
	if (file_name == NULL) { // No file name error - should not happen
		throw 1;
		return {};
	}
	std::ifstream file(file_name); // Open file from name
	if (file.is_open()) {
		bool headerFound = false;
		bool clauseFound = false;
		std::vector<std::vector<int>> clauses; 
		std::set<int> vars;
		std::string l;
		while( getline(file,l)) { 
			if (l[0] == 'c') {
				// Comment Line, do nothing
			} else if (l[0] == 'p') {
				headerFound = true;
				// Description, but our method of parsing does not require us to know this in advance
			} else if ((isNum(l[0]) || l[0] == '-' )) { // This would indicate it's a clause line
				clauseFound = true;
				if (headerFound == false) { throw 4; }
				clauses.push_back(parseClause(l)); // This is a SC hack - if a clause is valid we autosimplify it
				if (clauses.back().empty()) { clauses.pop_back(); }
			 	else { addVars(vars,clauses.back()); } // Otherwise add the variables to the set
			} else { // If it's not a comment, descriptor or clause line, it's invalid
				if (DEBUG_PARSE) { std::cout << "Invalid line from the start"; }
				throw 3;
			}
		}
		if (DEBUG_PARSE) std::cout << "Returning from scope with size: " << clauses.size() << std::endl;
		if (clauseFound == false) { throw 5; } 
		return {clauses, vars};
	} else { // File can't open error
		throw 2;
		//return {};
	}
}

std::vector<int> parseClause(const std::string &s) {
	if (s.back() != '0') {
		if (DEBUG_PARSE) std::cout << "Clause line" << s << " not ending in 0";
		throw 3; // all valid clauses _must_ end in a 0
	}
	std::vector<int> clause;
	unsigned int idx, offset;
	idx = 0;
	while (idx < s.size()) {
		while (! ( isNum(s[idx]) || s[idx] == '-')) { // parse whitespace
			if (s[idx] != ' ') {
				if (DEBUG_PARSE) { std::cout << "Invalid whitespace while moving iterator"; }
				throw 3;
			}
			idx++;
		}
		offset = 0;
		if (s[idx] == '-') {	
			if (!isNum(s[idx+1])) {	
				if (DEBUG_PARSE) { std::cout << "No characters after a negative sign"; }
				throw 3; // Negative with no digit following it
			}
			offset += 1;
		}
		while (idx+offset < s.size() && isNum(s[idx+offset])) { // iterate until end of number
			offset += 1;
		}
		if (s[idx+offset] != ' ' && idx+offset < s.size())  {
			if (DEBUG_PARSE) { std::cout << "Invalid whitespace when adjusting offset"; }
			throw 3; // Non space seperator
		}
		if (DEBUG_PARSE) { std::cout << "Trying to convert to int: " << s.substr(idx,offset) << "\n"; }
		if (std::stoi(s.substr(idx,offset)) == 0) { // If this is the end of the string, return
			std::sort(clause.begin(),clause.end(),ABS_INT_COMP); // Sort by magnitude
			if (validClause(clause)) throw 6; // Last minute hack - throwing  means sat
			return clause;
		}
		clause.push_back(std::stoi(s.substr(idx,offset)));
		idx = idx+offset+1;
	}
	std::sort(clause.begin(),clause.end(),ABS_INT_COMP); 
	if (validClause(clause)) throw 6; // Last minute hack - if
	return clause;	
}

void updatePLP(std::set<int> &pureLiterals, std::set<int> &impureLiterals, const int i) {
	if (!impureLiterals.count(i)) {
		if (pureLiterals.count(i)) {
			pureLiterals.erase(-i);
			impureLiterals.insert(-i);
			impureLiterals.insert(i);
		} else {
			pureLiterals.insert(i);
		}
	}
}

bool propogateLogic(std::map<int,bool> &assignments, const std::vector<std::vector<int>> &clauses) {
	assignments.clear();
	return true;
	std::set<int> pureLiterals;
	std::set<int> impureLiterals;
	for (const std::vector<int> &v : clauses) {
		if (v.size() == 1) {
			if (assignments.count(v.front())) {
				if (v.front() < 0) {
					if (assignments[-v.front()] == true) return false; 
					updatePLP(pureLiterals,impureLiterals,v.front());
				} else {
					if (assignments[v.front()] == false) return false;
					updatePLP(pureLiterals,impureLiterals,v.front());
				}
			} else {
				if (v.front() < 0) { assignments[-v.front()] = false; }
				else { assignments[v.front()] = true; }
				updatePLP(pureLiterals,impureLiterals,v.front());
			}
		} else {
			for (auto i : v) {
				updatePLP(pureLiterals,impureLiterals,i);
			}
		}
	}
	for (auto i : pureLiterals) {
		if (assignments.count(i) || assignments.count(-i)) {
			if ((i < 0 && assignments[-i] == true) || (i > 0 && assignments[i] == false)) { return false; }
		} else {
			if (i < 0) { assignments[-i] = false; }
			else { assignments[i] = true; }
		}
	}
	return true;
}


void addVars(std::set<int> &vars, const std::vector<int> &clause) {
	for (const int i : clause) {
		if (DEBUG_PARSE) { std::cout << "Adding : " << abs(i) << "\n"; }
		vars.insert(abs(i));
	}
	return;	
}

bool validClause(const std::vector<int> &clause) { // This checks for contradictions in each local clause
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

bool hasEmptyClause(const std::vector<std::vector<int>> &clauses) {
	for (auto v : clauses) { if (v.empty()) return true; }
	return false;
}


/* 
 * Debug Functions
 * Here be dragons
 * 
 * This code is not relevant for solving but were used to debug previously
 */
void printAssignments(const std::map<int,bool> &dict) {
	assert((DEBUG_DPLL || DEBUG_PARSE) && "Debug call to printAssignments");
	std::cout << "Assignments are :\n";
	for (auto it = dict.begin(); it != dict.end(); it++) {
		std::cout << it->first << " -> " << it->second << ",";
	}
	std::cout << "\n";
	return;
}

void dumpVector(const std::vector<int> &vec) {
	assert((DEBUG_DPLL || DEBUG_PARSE) && "Debug call to dumpVector");
	std::cout << "Vec: ";
	for (const int i : vec) {
		std::cout << i << " ";
	}
	std::cout << "\n";
}	
