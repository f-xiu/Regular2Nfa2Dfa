#pragma once
#include <string>
#include <set>
#include <vector>

using namespace std;

// �ڵ㶨��
struct node
{
	char Name;
};

// �߶���
struct edge
{
	node Start;
	node End;
	char transchar; // ת������ Start-transchar-End
};

// NFA��DFA��Ԫ����
// �ýṹ����NFA����Ķ��壬��ͬʱҲ���ڹ�������е���ʱ��㹹��
struct cell
{
	std::string vertex;			//�ڵ㼯
	std::vector<edge> EdgeSet;	// �߼�
	size_t EdgeCount;				// ����
	set<char> transchar;		// ת���������Ƽ� (a,b,c...)
	node StartNode;				// ��ʼ�ڵ㣬Ĭ��ֻ��һ����ʼ���
	vector<node> EndNodeSet;	// ��ֹ�ڵ㼯������Thompson�㷨���ɵ�NFAֻ��һ����ֹ��㣬����DFA���ж��
	size_t NodeCount;
};

// NFA��DFA�ľ���ṹ����
struct Matrix
{
	std::string vertex;		// �ڵ����ƣ����СΪvertex[nodeCount]
	set<char> transchar;	 // ת���������Ƽ� (a,b,c...)
	std::vector<std::vector<std::string>> transet;
	int edgeCount;			 // ����ͳ��
	int nodeCount;			 // �ڵ�ͳ��
	node StartNode;			 // ��ʼ�ڵ�
	vector<node> EndNodeSet; // ��ֹ�ڵ㼯
	set<char>EndSet;
};