#pragma once
#include <string>
#include <set>
#include <vector>

using namespace std;

// 节点定义
struct node
{
	char Name;
};

// 边定义
struct edge
{
	node Start;
	node End;
	char transchar; // 转换函数 Start-transchar-End
};

// NFA与DFA单元定义
// 该结构体是NFA整体的定义，但同时也用于构造过程中的临时结点构成
struct cell
{
	std::string vertex;			//节点集
	std::vector<edge> EdgeSet;	// 边集
	size_t EdgeCount;				// 边数
	set<char> transchar;		// 转换函数名称集 (a,b,c...)
	node StartNode;				// 开始节点，默认只有一个开始结点
	vector<node> EndNodeSet;	// 终止节点集，对于Thompson算法生成的NFA只有一个终止结点，但是DFA会有多个
	size_t NodeCount;
};

// NFA与DFA的矩阵结构定义
struct Matrix
{
	std::string vertex;		// 节点名称，其大小为vertex[nodeCount]
	set<char> transchar;	 // 转换函数名称集 (a,b,c...)
	std::vector<std::vector<std::string>> transet;
	int edgeCount;			 // 边数统计
	int nodeCount;			 // 节点统计
	node StartNode;			 // 开始节点
	vector<node> EndNodeSet; // 终止节点集
	set<char>EndSet;
};