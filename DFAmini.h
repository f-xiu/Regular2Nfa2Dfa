#pragma once
#include <iostream>
#include<unordered_map>
#include "NFA.h"

/* ------------DFA��С��------------ */
// DFA��С��
Matrix DFA_minimize(const Matrix& dfa, std::unordered_map<int, string>& map);

// ��ʼ��һ�����ڵ��Ϊ���սἯ���͡����սἯ��������
vector<set<char>> init_groups(const Matrix& dfa);

// �ӡ��սἯ���͡����սἯ����ʼ�����з���
vector<set<char>> grouping(const Matrix& dfa, const vector<set<char>>& groups);

// �ж�s2�Ƿ���s1�Ӽ�
bool sub_set(const set<char>& s1, const set<char>& s2);
