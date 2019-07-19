#include <vector>
#include <string>
#include <map>
#include <set>

inline bool isNum(const char c) { return (c >= '0' && c <= '9'); }

template<typename T> 
inline T abs(T x) { return (x > 0) ? x : (-1)*x; }

/*
 * dpll() : runs dpll test on file
 *
 * file_path : pointer to filename
 * 
 * returns true if sat, false if unsat
 */
bool dpll(char* file_path);

// Can set pointer  for optional assignment 
bool dpllOpt(char *file_path, std::map<int,bool> *opt_assignment);

/*
 * dpllInner() : inner function running dpll routine
 *
 * clauses: a list of all the cnf clauses
 * unassigned: a set of all unassigned variables
 * assigned: a map of assignments
 *
 * returns true if satisfiable from current assignment
 */
bool dpllInner(const std::vector<std::vector<int>> &clauses, std::set<int> unassigned, std::map<int,bool> assigned);

// Opt Assignment in case yo want to extract set of satisfying assignments
bool dpllInner(const std::vector<std::vector<int>> &clauses, std::set<int> unassigned, std::map<int,bool> assigned, std::map<int,bool> *opt_assignment);

/*
 * parse() : parses a file in DIMACs format
 * 
 * file_path: path to file
 * 
 * returns a list of clauses and a set of all variables 
 * throws a cryptic numerical error on failure
 */
std::pair<std::vector<std::vector<int> >,std::set<int> > parse(char* file_path);

/* 
 * parseClause() : parses a variable line into meaningful data
 * 
 * line : the line being parsed
 * 
 * returns a list of all variables inside clause with negations
 * throws a cryptic numerical error on failure
 */
std::vector<int> parseClause(const std::string &line);

/*
 * propogateLogic() : will do PLP and UCP in the first pass.
 * 
 * assignments: this will be emptied and replaced with the new assignments
 * clauses: reference to the actual clauses we need
 *
 * returns true on success, false if conflict detected
 */
bool propogateLogic(std::map<int,bool> &assignments, const std::vector<std::vector<int>> &clauses); 

/* 
 * addVars() : adds vars to set
 * 
 * s: set of all vars
 * clause: the clause having vars added from
 */
void addVars(std::set<int> &s, const std::vector<int> &clause);

/* 
 * validClause() : returns true if clause is valid

 * clause : ...
 *
 * returns true if valid, false if not
 */
bool validClause(const std::vector<int> &clause);


/* 
 * invalidAssignment(): returns whether or not any assignments are invalid
 * 
 * clauses: list of all clauses
 * dict: list of all assignments
 * 
 * returns true if any clause is unsatisfied, false otherwise
 */
bool invalidAssignment(const std::vector<std::vector<int>>&clauses, const std::map<int,bool> &dict);

/* 
 * validClause(): returns if clause is valid
 *
 * clause: list of variables inside clause
 * dict: mapping of all assignedvariables to true values
 * 
 * returns true if at least one variable is not unsatisfied, false otherwise
 */
bool validClause(const std::vector<int> &clause,const std::map<int,bool> &dict);

/* 
 * validVar(): returns if a variable has a valid assignment
 * 
 * var: variable being compared (with negation)
 * dict: assignment of said va
 *
 * returns false if conflict, true otherwise
 */
bool validVar(const int var,const std::map<int,bool> &dict);

/* DEBUG
 * printAssignments() : dumps assignments
 *
 * m : map being dumped
 */
void printAssignments(const std::map<int,bool> &m);

/* DEBUG
 * dumpVector() : dumps a vector of integers
 *
 * vec: vector being dumped
 */
void dumpVector(const std::vector<int> &vec);


