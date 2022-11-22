

#include"DFAmini.h"
#include"Nfa2Dfa.h"
#include"PublicNfa.h"
#include"Regular2NFA.h"

#include<iostream>
using namespace std;

cell get_from_file() {
    vector<vector<string>> strs;
    vector<string> str;
    string s = "";
    ifstream fin;
    fin.open("./nfa.txt");

    fin >> s;
    str.push_back(s);
    while (fin >> s) {
        if (s.length() == 0)break;
        if (s.length() == 1) {
            strs.push_back(str);
            vector<string>tm;
            str = tm;
        }
        str.push_back(s);
    }
    strs.push_back(str);

    cell cellNfa;
    cellNfa.EdgeCount = 0;
    cellNfa.NodeCount = 0;
    node n;
    n.Name = 'X';
    node end;
    end.Name = 'Y';
    cellNfa.StartNode = n;
    cellNfa.EndNodeSet.push_back(end);
    for (int i = 0; i < strs.size(); i++)
    {
        node n;
        edge e;
        n.Name = strs[i][0][0];
        str = strs[i];
        cellNfa.NodeCount++;
        for (int j = 1; j < str.size(); j++)
        {
            edge ed;
            cellNfa.EdgeCount++;
            char c = str[j][2];
            if (c != '~') cellNfa.transchar.insert(c);
            ed.transchar = c;
            ed.Start = n;
            node tm;
            tm.Name = str[j][5];
            ed.End = tm;
            cellNfa.EdgeSet.push_back(ed);
        }
    }
    
    return cellNfa;
}

void rec_words(Matrix&dfa,vector<string> ans) {
    string s;
    for (string p : ans) {
        s = p;
        int i = find_index_in_vertex(dfa,dfa.StartNode.Name);
        int err_flag = 0;
        for (int j = 0;j < s.length();j++) {
            int flag = 0;
            for (int m = 0;m < dfa.nodeCount;m++) {
                if (dfa.transet[i][m][0] == s[j]) {
                    i = m;
                    flag = 1;
                    break;
                }
            }
            if (!flag) {
                err_flag = 1;
                break;
            }
            else {
                cout << s[j] << endl;
            }
        }
        if (err_flag||dfa.EndSet.find(dfa.vertex[i]) == dfa.EndSet.end()) {
            cout << "error" << endl;
        }
        else {
            cout << "pass" << endl;
        }
            cout << "--------------------------" << endl;

    }
}

int main(void) {
	//---------------正规式输入到NFACELL-----------------------
	string regex = input_regex(); // (a|b)*a|bcd

	string separated_regex = add_separation(regex);

	string postfix_regex = to_postfix(separated_regex);

	//cell NFA_cell = regex_to_nfa(postfix_regex);
    grup g= getNFA(postfix_regex);
    printNFA(g);

    cell NFA_cell = get_from_file();

	//-----------------------------------------------------------------
	Matrix nfa = cell2matrix(NFA_cell);
	

	Matrix dfa = NFA2DFA(nfa);
	

	unordered_map<int, string> map;
	Matrix dfa_m = DFA_minimize(dfa,map);
	printDfa(dfa_m,map);

	cout << "输入识别的单词,一行,用空格隔开"<<endl;
    vector<string>ans;
    string s;
    while(cin>>s) {
        ans.push_back(s);
        if (getchar() == '\n') {
            break;
        }
    }

    rec_words(dfa_m,ans);

	return 0;
}