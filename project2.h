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
void RemoveUselessSymbols();
void CalculateFollowSets();
void CheckIfGrammarHasPredictiveParser();
int main (int argc, char* argv[]);

#endif
