#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#define MAX 100
using namespace std;

// NFA边
struct Edge
{
	int start;
	int end;
	char accept;
};

// NFA单元，一个大的NFA单元可以是由很多小单元通过规则拼接起来
class grup
{
public:
	vector<Edge> Edges; //这个NFA拥有的边
	int stateCount;		//状态数
	int StartState;		//开始状态
	int EndState;		//结束状态
public:
	grup()
	{
		this->stateCount = 2;
		this->StartState = 0;
		this->EndState = 1;
	}
};


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

//根据正规式获得NFA
grup getNFA(string text);

void printNFA(grup out);