#include "PublicNfa.h"
#include <stack>
#include <vector>
#include <queue>
#include<unordered_map>
#include<algorithm>
using namespace std;


// 将cell转为矩阵形式
Matrix cell2matrix(const cell& nfa_cell)
{
	int i, j;
	Matrix matrix;
	matrix.edgeCount = 0;
	auto end_iter = nfa_cell.EndNodeSet.begin();

	//结点数量同cellnode
	//matrix.nodeCount = maxName - 65 + 1; // 统计字母数量
	matrix.nodeCount = nfa_cell.NodeCount;
	// 初始化矩阵节点名称
	matrix.vertex.resize(matrix.nodeCount);
	for (i = 0;i < matrix.nodeCount;i++)
	{
		matrix.vertex[i] = nfa_cell.vertex[i];
	}
	//matrix.vertex = nfa_cell.vertex;

	// 初始化二维矩阵
	matrix.transet.resize(matrix.nodeCount);
	for (i = 0; i < matrix.nodeCount; i++)
	{
		matrix.transet[i].resize(matrix.nodeCount);
	}
	for (i = 0; i < matrix.nodeCount; i++)
	{
		for (j = 0; j < matrix.nodeCount; j++)
		{
			matrix.transet[i][j] = '$'; // '$'表示无数据
		}
	}
	matrix.StartNode = nfa_cell.StartNode;
	matrix.EndNodeSet = nfa_cell.EndNodeSet;
	for (node n : matrix.EndNodeSet) {
		matrix.EndSet.insert(n.Name);
	}
	matrix.transchar = nfa_cell.transchar;


	for (i = 0; i < nfa_cell.EdgeCount; i++)
	{
		int start, end;
		start = find_index_in_vertex(matrix, nfa_cell.EdgeSet[i].Start.Name);
		end = find_index_in_vertex(matrix, nfa_cell.EdgeSet[i].End.Name);
		if (start != -1 && end != -1)
		{
			if (matrix.transet[start][end] == "$")
			{
				matrix.transet[start][end] = "";
			}
			matrix.transet[start][end] += nfa_cell.EdgeSet[i].transchar;
			matrix.edgeCount++;
		}
		else
		{
			cerr << "error in transfer from cell to matrix!\n";
			return Matrix(matrix);
		}
	}

	return matrix;
}

// 找到节点在矩阵名称中的位置，返回下标
int find_index_in_vertex(const Matrix& matrix, char ch)
{
	int i;
	for (i = 0; i < matrix.nodeCount; i++)
	{
		if (matrix.vertex[i] == ch)
		{
			return i;
		}
	}
	return -1;
}

// 返回一个节点的ε闭包
set<char> e_closure(const Matrix& nfa, char start_node)
{
	int index;
	set<char> result;

	stack<char> s;
	s.push(start_node);

	while (!s.empty())
	{
		char ch = s.top();
		s.pop();
		result.insert(ch);
		index = find_index_in_vertex(nfa, ch);
		for (int i = 0; i < nfa.nodeCount; i++)
		{
			if (nfa.transet[index][i] == "~")
			{
				s.push(nfa.vertex[i]);
			}
		}
	}

	return result;
}


// 返回一个节点经过转换字符转换得到的结果（ε闭包结果）
set<char> transfer_result(const Matrix& nfa, char start, char transchar)
{
	set<char> result;
	int index = find_index_in_vertex(nfa, start);
	for (int i = 0; i < nfa.nodeCount; i++)
	{
		if (nfa.transet[index][i].find(transchar) != string::npos)
		{
			// 将其ε-闭包加入结果
			result = union_set(result, e_closure(nfa, nfa.vertex[i]));
		}
	}
	return result;
}
// 将两个集合合并
set<char> union_set(const set<char>& s1, const set<char>& s2)
{
	set<char> temp(s1.begin(), s1.end());
	temp.insert(s2.begin(), s2.end());
	return temp;
}

// 判断两个集合是否相同
bool equal_set(const set<char>& s1, const set<char>& s2)
{
	// 大小不一样的集合必然不同
	if (s1.size() != s2.size())
	{
		return false;
	}

	set<char> temp;
	temp = union_set(s1, s2);

	if (temp.size() != s1.size())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void out(const Matrix& dfa, int k, unordered_map<int, string>& map)
{
	cout << map[k] << " ";
	for (auto iter = dfa.transchar.begin();iter != dfa.transchar.end();iter++) {
		for (int j = 0;j < dfa.nodeCount;j++) {
			string tm = "";
			tm += *iter;
			if (dfa.transet[k][j] == tm) {
				cout << map[k] << "-" << *iter << "->" << map[j] << " ";
			}
		}
	}
	cout << endl;
}

void printDfa(const Matrix &dfa, unordered_map<int, string>&map) {
	int index = find_index_in_vertex(dfa, dfa.StartNode.Name);
	out(dfa, index, map);


	for (int end = 0;end < dfa.nodeCount;end++) {
		if (dfa.EndSet.find(dfa.vertex[end]) == dfa.EndSet.end())continue;
		out(dfa, end, map);
	}
	
	for (int k = 0;k < dfa.nodeCount;k++) {
		if (dfa.EndSet.find(dfa.vertex[k]) == dfa.EndSet.end() && dfa.vertex[k] != dfa.StartNode.Name) {
			out(dfa, k, map);
		}
	}
}