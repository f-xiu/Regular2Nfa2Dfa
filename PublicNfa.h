#pragma once
#include <iostream>
#include<unordered_map>
#include"NFA.h"

// ��NFA_cellתΪ������ʽ
Matrix cell2matrix(const cell& nfa_cell);

// �ҵ��ڵ��ھ��������е�λ�ã������±�
int find_index_in_vertex(const Matrix& matrix, char ch);


// ����һ���ڵ㾭��ת���ַ�ת���õ��Ľ�����űհ������
set<char> transfer_result(const Matrix& nfa, char start, char transchar);

// ���������Ϻϲ�
set<char> union_set(const set<char>& s1, const set<char>& s2);


// ����һ���ڵ�Ħűհ�
set<char> e_closure(const Matrix& nfa, char start_node);

// �ж����������Ƿ���ͬ
bool equal_set(const set<char>& s1, const set<char>& s2);

void out(const Matrix& dfa, int k, unordered_map<int, string>& map);

void printDfa(const Matrix& dfa, unordered_map<int, string>& map);
