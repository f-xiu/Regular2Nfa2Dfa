#pragma once
#include <iostream>
#include<unordered_map>
#include "NFA.h"

/* ------------DFA最小化------------ */
// DFA最小化
Matrix DFA_minimize(const Matrix& dfa, std::unordered_map<int, string>& map);

// 初始化一个将节点分为“终结集”和“非终结集”的容器
vector<set<char>> init_groups(const Matrix& dfa);

// 从“终结集”和“非终结集”开始，进行分组
vector<set<char>> grouping(const Matrix& dfa, const vector<set<char>>& groups);

// 判断s2是否是s1子集
bool sub_set(const set<char>& s1, const set<char>& s2);
