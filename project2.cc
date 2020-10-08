/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *               Rida Bazzi 2019
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "lexer.h"
#include "project2.h"

using namespace std;

vector<string> symbols;
unordered_map<string, int> symbolmap;
vector<rule*> rules;
unordered_set<int> nonterminals;
unordered_set<int> terminals;
int sym_index;
LexicalAnalyzer lexer;

Token expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error(__LINE__);

    return t;
}

void syntax_error(int lineno)
{
    cout << "SYNTAX ERROR !!!\n";
	//printf("called from line number: %d\n", lineno);
    exit(1);
}
		
void parseIDList(rule* r) {
	Token t = expect(ID);

	if (symbolmap.count(t.lexeme) == 0) {
		// add symbol to universe container
		symbols.push_back(t.lexeme);

		// update symbol table with the symbol's index in the universe container, 
		// indicating that the symbol has been found
		symbolmap[t.lexeme] = sym_index;

		// add symbol to terminals set by default
		terminals.insert(sym_index);

		sym_index += 1;

	} 
		
	r->RHS.push_back(symbolmap[t.lexeme]);

	t = lexer.peek(1);

	if (t.token_type == ID)
		parseIDList(r);
	else if (t.token_type != STAR)
		syntax_error(__LINE__);
}

void parseRHS(rule* r) {
	Token t = lexer.peek(1);
	if (t.token_type == ID)
		parseIDList(r);
	else if (t.token_type == STAR)
		r->RHS.push_back(0);
	else
		syntax_error(__LINE__);
}

void parseRule() {
	rule* r = new rule;
	Token t = expect(ID);

	if (symbolmap.count(t.lexeme) == 0) {
		// add symbol to universe container
		symbols.push_back(t.lexeme);

		// update symbol table with the symbol's index in the universe container, 
		// indicating that the symbol has been found
		symbolmap[t.lexeme] = sym_index;

		// add symbol to nonterminals set
		nonterminals.insert(sym_index);

		sym_index += 1;
	} else if (nonterminals.find(symbolmap[t.lexeme]) == nonterminals.end()) {
		// if the current symbol is known, but is not in the nonterminals set,
		// add the symbol to the nonterminals set
		nonterminals.insert(symbolmap[t.lexeme]);

		// remove the symbol from the terminals set
		terminals.erase(symbolmap[t.lexeme]);
	}

	r->LHS = symbolmap[t.lexeme];

	expect(ARROW);

	parseRHS(r);

	expect(STAR);

	rules.push_back(r);
}

void parseRuleList() {
	parseRule();
	Token t = lexer.peek(1);
	if (t.token_type == ID) {
		parseRuleList();
	} else if (t.token_type != HASH)
		syntax_error(__LINE__);
}

void parseGrammar() {
	parseRuleList();
	expect(HASH);
	expect(END_OF_FILE);
}

// read grammar
void ReadGrammar()
{
	symbols.push_back("#");
	symbols.push_back("$");
	sym_index = symbols.size();
	parseGrammar();
}

// Task 1
void printTerminalsAndNoneTerminals()
{
	vector<int> terms;
	vector<int> nonterms;

	for (int i = 2; i < symbols.size(); i++) {
		if(nonterminals.find(symbolmap[symbols[i]]) == nonterminals.end())
			terms.push_back(symbolmap[symbols[i]]);
		else
			nonterms.push_back(symbolmap[symbols[i]]);
	}

	for (int i = 0; i < terms.size(); i++)
		cout << symbols[terms[i]] << " ";

	for (int i = 0; i < nonterms.size(); i++)
		cout << symbols[nonterms[i]] << " ";
}

void calcGeneratingSymbols(vector<rule*>* result) {
	
	// initialization
	
	int genArray[symbols.size()] = {0};
	genArray[0] = 1;
	for (auto it = terminals.begin(); it != terminals.end(); it++) {
		genArray[*it] = 1;
	}

	// begin iteration
	
	bool changed = true;

	while(changed) {
		changed = false;

		// iterate through every rule
		for (int i = 0; i < rules.size(); i++) {
			bool generates = true;

			// iterate through every symbol of the current rule
			for (auto it = rules[i]->RHS.begin(); it != rules[i]->RHS.end(); it++) {
				if (genArray[*it] == 0) {
					generates = false;
					break;
				}
			}

			// if the current rule generates, 
			// denote it as having said attribute in genArray
			if (generates && genArray[rules[i]->LHS] != 1) {
				genArray[rules[i]->LHS] = 1;
				changed = true;
			}
		}
	}

	// remove rules with non-generating symbols
	
	// iterate through every rule
	for (int i = 0; i < rules.size(); i++) {
		bool generates = true;

		// iterate through every symbol of the current rule and check if even a
		// single symbol within the current rule doesn't generate
		for (auto it = rules[i]->RHS.begin(); it != rules[i]->RHS.end(); it++) {

			// if a symbol in the current rule doesn't generate, 
			// don't add the rule to the result vector
			if (genArray[*it] == 0) {
				generates = false;
				break;
			}
		}

		// if the current rule generates, 
		// add it to the resulting vector of rules
		if (generates) {
			result->push_back(rules[i]);
		}
	}
}

void calcReachableSymbols(vector<rule*>* result) {

	// initialize
	
	int reachArr[symbols.size()] = {0};

	if (result->size() != 0) {
		reachArr[(*result)[0]->LHS] = 1;

		// iterate through each rule, adding all 
		// symbols of a given rule if the LHS is reachable
		for (int i = 0; i < result->size(); i++) {
			if(reachArr[(*result)[i]->LHS] == 1) {
				for (auto it = (*result)[i]->RHS.begin(); it != (*result)[i]->RHS.end(); it++) {
					reachArr[*it] = 1;
				}
			}
		}
		
		// create vector for all reachable rules
		vector<rule*>* newResult = new vector<rule*>;

		for (auto it = result->begin(); it != result->end(); it++) {
			if (reachArr[(*it)->LHS] == 1) {
				newResult->push_back(*it);
			}
		}

		result->clear();
		result->insert(result->end(), newResult->begin(), newResult->end());
	}
}

// Task 2 test
void RemoveUselessSymbols() {
	vector<rule*>* result = new vector<rule*>;
	calcGeneratingSymbols(result);
	calcReachableSymbols(result);

	if ((result->size() != 0) && ((*result)[0]->LHS == rules[0]->LHS)) {
		for (int i = 0; i < result->size(); i++) {
			cout << symbols[(*result)[i]->LHS] << " ->";
			for (auto it = (*result)[i]->RHS.begin(); it != (*result)[i]->RHS.end(); it++)
				cout << " " << symbols[*it];
			cout << endl;
		}
	}
}

// this function performs a + b, for a and b are both sets
// if mode == 1: add (a - {epsilon}) to b
// if mode == 0: add a to b
bool addSet (unordered_set<int>* a, unordered_set<int>* b, int mode) {
	bool changed = false;
	int origSize = b->size();

	for (auto it = a->begin(); it != a->end(); it++) {
		if (mode && (*it == 0))
			continue;
		else
			b->insert(*it);
	}

	if (b->size() > origSize)
		changed = true;

	return changed;
}

// this function prints the contents of FIRST and FOLLOW sets
// mode == 1: print FIRST sets
// mode == 0: print FOLLOW sets
void printSets (std::unordered_map<int, std::unordered_set<int>>* s, int mode) {
	vector<int> terms;
	vector<int> nonterms;
	string setName;

	if (mode)
		setName = "FIRST";
	else
		setName = "FOLLOW";

	for (int i = 2; i < symbols.size(); i++) {
		if(nonterminals.find(symbolmap[symbols[i]]) == nonterminals.end())
			terms.push_back(symbolmap[symbols[i]]);
		else
			nonterms.push_back(symbolmap[symbols[i]]);
	}

	for (int i = 0; i < nonterms.size(); i++) {

		cout << setName << "(" << symbols[nonterms[i]] << ") = { ";

		if (mode) {
			bool firstFound = false;
			for (int j = 0; j < symbols.size(); j++) {
				if ((*s)[nonterms[i]].find(j) != (*s)[nonterms[i]].end())
					if (!firstFound) {
						cout << symbols[j];
						firstFound = true;
					} else
						cout << ", " << symbols[j];
			}
			cout << " }" << endl;

		} else {

		}
	}
}

// Task 3
unordered_map<int, unordered_set<int>>* CalculateFirstSets() {
	unordered_map<int, unordered_set<int>>* FIRST = new unordered_map<int, unordered_set<int>>;

	// apply rule I and II by
	// initializing FIRST[x] = x for all terminals and epsilon

	(*FIRST)[0].insert(0);
	for (auto it = terminals.begin(); it != terminals.end(); it++) {
		(*FIRST)[*it].insert(*it);
	}

	// begin iterating through each rule, applying rules III, IV, and V
	// in successive order until there are no further changes made to FIRST
	bool changed = true;
	while (changed) {
		changed = false;
		for (auto it = rules.begin(); it != rules.end(); it++) { 
			
			// apply rule III
			
			if(addSet(&(*FIRST)[(*it)->RHS.at(0)], &(*FIRST)[(*it)->LHS], 1)) {
				if (!changed)
					changed = true;
			}
			
			// apply rule IV
			bool fiveApplies = false;
			for (int i = 0; i < (*it)->RHS.size(); i++) {
				// check if epsilon is in FIRST[current symbol]
				if (((*FIRST).count((*it)->RHS[i]) != 0) && ((*FIRST)[(*it)->RHS[i]].find(0) != (*FIRST)[(*it)->RHS[i]].end())) {

					// if it is, then add the next symbol's FIRST set - {epsilon}
					// to FIRST[LHS]
					int next = i + 1;
					if (next < (*it)->RHS.size()) {
						if (changed) 
							addSet(&(*FIRST)[(*it)->RHS[next]], &(*FIRST)[(*it)->LHS], 1);
						else
							changed = addSet(&(*FIRST)[(*it)->RHS[next]], &(*FIRST)[(*it)->LHS], 1);
					} else if (next == (*it)->RHS.size()) 
						fiveApplies = true;
				} else
					break;
			}

			// apply rule V
			if (fiveApplies) {
				if (changed)
					addSet(&(*FIRST)[0], &(*FIRST)[(*it)->LHS], 0);
				else
					changed = addSet(&(*FIRST)[0], &(*FIRST)[(*it)->LHS], 0);
			}

			if (changed)
				cout << endl;
		}
	}

	return FIRST;
}

// Task 4
void CalculateFollowSets() {
    cout << "4\n";
}

// this function performs "a intersect b", (for a,b are sets) and
// returns 0 if the result is the empty set, and returns 1 if the
// result is not the empty set
bool intersectSets(unordered_set<int>* a, unordered_set<int>* b) {
	unordered_set<int>* larger = a;
	unordered_set<int>* smaller = b;
	bool isNil = true;

	// if the b set is larger, specify it as such
	if(b->size() > a->size()) {
		larger = b;
		smaller = a;
	}

	for (auto it = larger->begin(); it != larger->end(); it++) {
		if (smaller->find(*it) != smaller->end()) {
			isNil = false;
			break;
		}
	}
	return isNil;
}

// Task 5
void CheckIfGrammarHasPredictiveParser() {
	vector<rule*>* vec = new vector<rule*>;
	calcGeneratingSymbols(vec);
	calcReachableSymbols(vec);

	if(vec->size() != rules.size())
		cout << "NO" << endl;
	else {
		cout << "YES" << endl;
		//unordered_map<int, unordered_set<int>> = CalculateFirstSets();
	}
}
    
int main (int argc, char* argv[]) {

    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    /*
       Note that by convention argv[0] is the name of your executable,
       and the first argument to your program is stored in argv[1]
     */

    task = atoi(argv[1]);
    
    ReadGrammar();  // Reads the input grammar from standard input
                    // and represent it internally in data structures
                    // ad described in project 2 presentation file

    switch (task) {
        case 1: printTerminalsAndNoneTerminals();
            break;

        case 2: RemoveUselessSymbols();
            break;

        case 3: 
			{
				unordered_map<int, unordered_set<int>>* firstSets = CalculateFirstSets();
				printSets(firstSets, 1);
				break;
			}

        case 4: CalculateFollowSets();
            break;

        case 5: CheckIfGrammarHasPredictiveParser();
            break;

        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}

