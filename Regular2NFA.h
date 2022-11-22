#pragma once
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include <string>
#define MAX 100
using namespace std;

// NFA��
struct Edge
{
	int start;
	int end;
	char accept;
};

// NFA��Ԫ��һ�����NFA��Ԫ�������ɺܶ�С��Ԫͨ������ƴ������
class grup
{
public:
	vector<Edge> Edges; //���NFAӵ�еı�
	int stateCount;		//״̬��
	int StartState;		//��ʼ״̬
	int EndState;		//����״̬
public:
	grup()
	{
		this->stateCount = 2;
		this->StartState = 0;
		this->EndState = 1;
	}
};


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

//��������ʽ���NFA
grup getNFA(string text);

void printNFA(grup out);