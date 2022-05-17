# Grammar Analysis Tool
## Description
This tool is used to perform the following tasks on a user-provided input grammar: 

1. Determine terminals and non-terminals
2. Find useless symbols and remove rules with said symbols
3. Compute first sets 
4. Compute follow sets
5. Determine if the grammar has a predictive parser

The results of the listed tasks are printed to stdout.

## Usage
This program requires a numeric argument that denotes which of the 5 tasks is to be performed on the grammar, then accepts the grammar file to be operated on via input redirection.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** ```$ ./a.out 2 < example_grammar.txt```

A PL grammar is represented in a text file with the following format:
```
 <name> -> <symbol list> *
 ...
 <name> -> <symbol list> *
 #
```
where ```<name>``` is the rule name, and ```<symbol list>``` is a list of terminals and non-terminals delimited by spaces. In addition, every rule is terminated by an asterisk, and the last rule is followed by an octothorpe.
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** example_grammar.txt

```
  decl -> idList colon ID *
  idList -> ID idList1 *
  idList1 -> *
  idList1 -> COMMA ID idList1 *
  #
```
  
## Functionality
### Task 1: List Terminals and Nonterminals
  
Task one simply outputs the list of terminals in the order in which they appear in the grammar rules followed by the list of non-terminals in the order in which they appear in the grammar rules.
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** For the input grammar

```
  decl -> idList colon ID *
  idList -> ID idList1 *
  idList1 -> *
  idList1 -> COMMA ID idList1 *
  #
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The output for task 1 is:

``` colon ID COMMA decl idList idList1```

### Task 2: Eliminate Useless Symbols
  
The following is the definition for useless and useful symbols

**Definition - Useful Symbols:** Symbol ```A``` is _useful_ if there is a derivation starting from _S_ of a string of terminals, possibly empty, in which ```A``` appears:
  
```
  S *-> ... *-> xAy -> ... *-> w ∈ T*
```
  
**Definition - Useless Symbols:** A symbol is _useless_ if it is not useful.

Once the useless symbols in the grammar are determined (if any), the rules that contain said useless symbols are removed from the grammar. Each of the remaining useful rules are printed on a new line in the same relative order that they were in with the following format:

```
  <LHS> -> <RHS>
```

Where ```<LHS>``` is the name of the grammar rule and ```<RHS>``` is the right-hand side of the grammar rule. If the grammar rule is of the form A -> ε, use # to represent the epsilon. Note that this is different from the input format.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** Given the grammar:

```
  S -> A B *
  S -> C *
  C -> c *
  S -> a *
  A -> a A *
  B -> b *
  #
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;the output for task 2 will be:

```
  S -> C
  C -> c
  S -> a
```

### Task 3: Calculate First Sets
  
This task computes the first sets for all the non-terminal symbols. Then, for each of the non-terminals of the input grammar, in the order that it appears in the grammar, one line is output in the following format:
  
```
  FIRST(<symbol>) = { <set_items> }
```

where ```<symbol>``` is replaced by the non-terminal name and ```<set_items>``` is  replaced by a comma-separated list of elements of the set ordered in the following manner:

* If ε belongs to the set, represent it as #.
* If ε belongs to the set, it should be listed before any other element of the set.
* All other elements of the set should be sorted in the order in which they appear in the grammar
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** Given the grammar:

```
  decl -> idList colon ID *
  idList -> ID idList1 *
  idList1 -> *
  idList1 -> COMMA ID idList1 *
  #
```
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The output for task 3 will be:
  
```
  FIRST(decl) = { ID }
  FIRST(idList) = { ID }
  FIRST(idList1) = { #, COMMA }
```
  
### Task 4: Calculate Follow Sets
  
In Task 4, follow sets are computed for each of the non-terminals in the input grammar. Then, in the order that the non-terminals appear in the grammar, one line is output in the following format:
  
```
  FOLLOW(<symbol>) = { <set_items> }
```

where ```<symbol>``` should be replaced by the non-terminal and ```<set_items>``` should be replaced by the comma-separated list of elements of the set ordered in the following manner.
  
* If EOF belongs to the set, it will be represented as $.
* If EOF belongs to the set, it will be listed before any other element of the set.
* All other elements of the set should be sorted in the order in which they appear in the grammar.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** Given the grammar:

```
  decl -> idList colon ID *
  idList -> ID idList1 *
  idList1 -> *
  idList1 -> COMMA ID idList1 *
  #
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The output for task 4 will be:

```
  FOLLOW(decl) = { $ }
  FOLLOW(idList) = { colon }
  fFOLLOW(idList1) = { colon }
```

### Task 5: Determine Predictive Parser
  
  Task 5 determines if the grammar has a predictive parser and outputs either ```YES``` or ```NO``` accordingly. If the grammar has useless symbols, the tool will output ```NO```.
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Example:** Given the grammar:

```
  decl -> idList colon ID *
  idList -> ID idList1 *
  idList1 -> *
  idList1 -> COMMA ID idList1 *
  #
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The output for task 5 is: ```YES```
