#ifndef _PROJECT_2_H
#define _PROJECT_2_H

typedef struct rule {
	int LHS;
	std::vector<int> RHS;
} rule;

Token expect(TokenType expected_type);
void syntax_error(int lineno);
void parseIDList(rule* r);
void parseRHS(rule* r);
void parseRule();
void parseRuleList();
void parseGrammar();
void ReadGrammar();
void printTerminalsAndNoneTerminals();
void calcGeneratingSymbols (std::vector<rule*>* result);
void calcReachableSymbols (std::vector<rule*>* result);
void RemoveUselessSymbols();
bool addSet (std::unordered_set<int>* a, std::unordered_set<int>* b, int mode);
bool intersectSets(std::unordered_set<int>* a, std::unordered_set<int>* b);
void printSets (std::unordered_map<int, std::unordered_set<int>>* s);
std::unordered_map<int, std::unordered_set<int>>* CalculateFirstSets();
void CalculateFollowSets();
void CheckIfGrammarHasPredictiveParser();
int main (int argc, char* argv[]);

#endif
