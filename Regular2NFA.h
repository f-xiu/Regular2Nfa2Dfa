#pragma once
#include <iostream>
#include "NFA.h"

/* ------------��������ʽ------------ */
// ��������ʽ
string input_regex();

// ������������ʽ�Ƿ�Ϸ�
bool check_legal(const string& regex);

// �������ʽ�Ƿ��зǷ��ַ�
bool check_character(const string& regex);

// �������ʽ�����űպ�
bool check_parenthesis(const string& regex);

// �Ƿ��ǺϷ��ַ�
bool is_letter(char c);

// �Ƿ��������
bool is_operator(char c);

/* ------------��׺ת��׺------------ */
// Ϊ��׺���ʽ��ӷָ���(��+��)
string add_separation(const string& regex);

// ��׺���ʽת��׺���ʽ
string to_postfix(string& regex);

// isp
int isp(char c);

// icp
int icp(char c);

/* ------------����ʽ����NFA------------ */
// ����ʽ����nfa
cell regex_to_nfa(const string& regex);

// ��ʼ���ڵ�
// ��Ҫ���ڸ��ݶ����ַ�����һ���ڵ�
cell init_cell(char ch);

// ������(a|b)
cell unite_cell(const cell& Left, const cell& Right);

// ������(ab)
cell join_cell(const cell& Left, const cell& Right);

// �հ�����(a*)
cell loop_cell(const cell& Left);

// ��c2�еı߿�����c1
void copy_cell(cell& c1, const cell& c2);


// ��NFA_cellתΪ������ʽ
Matrix cell2matrix(const cell& nfa_cell);
