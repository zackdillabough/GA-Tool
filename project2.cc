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

// Task 2 test
void RemoveUselessSymbols()
{
    cout << "2\n";
}

// Task 3
void CalculateFirstSets()
{
    cout << "3\n";
}

// Task 4
void CalculateFollowSets()
{
    cout << "4\n";
}

// Task 5
void CheckIfGrammarHasPredictiveParser()
{
    cout << "5\n";
}
    
int main (int argc, char* argv[])
{
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

        case 3: CalculateFirstSets();
            break;

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

