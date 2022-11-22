#pragma once
#include <iostream>
#include<unordered_map>
#include"NFA.h"

// 将NFA_cell转为矩阵形式
Matrix cell2matrix(const cell& nfa_cell);

// 找到节点在矩阵名称中的位置，返回下标
int find_index_in_vertex(const Matrix& matrix, char ch);


// 返回一个节点经过转换字符转换得到的结果（ε闭包结果）
set<char> transfer_result(const Matrix& nfa, char start, char transchar);

// 将两个集合合并
set<char> union_set(const set<char>& s1, const set<char>& s2);


// 返回一个节点的ε闭包
set<char> e_closure(const Matrix& nfa, char start_node);

// 判断两个集合是否相同
bool equal_set(const set<char>& s1, const set<char>& s2);

void out(const Matrix& dfa, int k, unordered_map<int, string>& map);

void printDfa(const Matrix& dfa, unordered_map<int, string>& map);
