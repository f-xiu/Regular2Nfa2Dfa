#pragma once
#include <iostream>
#include "NFA.h"

/* ------------输入正规式------------ */
// 输入正规式
string input_regex();

// 检查输入的正规式是否合法
bool check_legal(const string& regex);

// 检查正规式是否有非法字符
bool check_character(const string& regex);

// 检查正规式的括号闭合
bool check_parenthesis(const string& regex);

// 是否是合法字符
bool is_letter(char c);

// 是否是运算符
bool is_operator(char c);

/* ------------中缀转后缀------------ */
// 为中缀表达式添加分隔符(‘+’)
string add_separation(const string& regex);

// 中缀表达式转后缀表达式
string to_postfix(string& regex);

// isp
int isp(char c);

// icp
int icp(char c);

/* ------------正规式生成NFA------------ */
// 正规式生成nfa
cell regex_to_nfa(const string& regex);

// 初始化节点
// 主要用于根据读入字符生成一个节点
cell init_cell(char ch);

// 或运算(a|b)
cell unite_cell(const cell& Left, const cell& Right);

// 与运算(ab)
cell join_cell(const cell& Left, const cell& Right);

// 闭包运算(a*)
cell loop_cell(const cell& Left);

// 将c2中的边拷贝到c1
void copy_cell(cell& c1, const cell& c2);


// 将NFA_cell转为矩阵形式
Matrix cell2matrix(const cell& nfa_cell);
