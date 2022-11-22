

#include"DFAmini.h"
#include"Nfa2Dfa.h"
#include"PublicNfa.h"
#include"Regular2NFA.h"

#include<iostream>
using namespace std;

int main(void) {
	//---------------正规式输入到NFACELL-----------------------
	string regex = input_regex(); // (a|b)*a|bcd

	string separated_regex = add_separation(regex);

	string postfix_regex = to_postfix(separated_regex);

	cell NFA_cell = regex_to_nfa(postfix_regex);
	//-----------------------------------------------------------------
	Matrix nfa = cell2matrix(NFA_cell);
	

	Matrix dfa = NFA2DFA(nfa);
	

	unordered_map<int, string> map;
	Matrix dfa_m = DFA_minimize(dfa,map);
	printDfa(dfa_m,map);

	return 0;
}