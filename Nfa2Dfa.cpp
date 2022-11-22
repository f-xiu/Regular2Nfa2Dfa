#include "Nfa2Dfa.h"
#include "PublicNfa.h"
#include <stack>
#include <vector>
#include <queue>
#include<set>
using namespace std;

// 将NFA_matrix转换为DFA_matrix
Matrix NFA2DFA(const Matrix& nfa)
{
	Matrix dfa;
	vector<set<char>> Q;

	// 我们应在Q集合中元素数量不再变化时停止循环，运用一个队列结构来实现
	queue<set<char>> s;
	// 初始时将开始节点的ε闭包入队列
	s.push(e_closure(nfa, nfa.StartNode.Name));
	Q.push_back(e_closure(nfa, nfa.StartNode.Name));

	// 另外最好能保存转换结果，这里用一个队列来完成
	queue<set<char>> trans_edge;

	// 子集法获取新节点
	while (!s.empty())
	{
		set<char> top = s.front(); // 取出队首集合
		s.pop();

		// 对每一个转换字符，求转换结果
		set<char>::iterator transchar_iter;
		for (transchar_iter = nfa.transchar.begin(); transchar_iter != nfa.transchar.end(); transchar_iter++)
		{

			set<char> temp;
			int flag = 0;

			// 遍历所取出集合中的每一个元素，求其转换结果的ε-闭包，并合并到temp中
			set<char>::iterator top_iter;
			for (top_iter = top.begin(); top_iter != top.end(); top_iter++)
			{
				temp = union_set(temp, transfer_result(nfa, *top_iter, *transchar_iter));
			}

			if (temp.size() == 0)
			{
				//如果结果结果为空，则会以$表示
				temp.insert('$');
			}
			else
			{
				// 遍历Q，看是否已有与temp相同的集合，没有则加入栈中
				vector<set<char>>::iterator Q_iter;
				for (Q_iter = Q.begin(); Q_iter != Q.end(); Q_iter++)
				{
					if (equal_set(*Q_iter, temp))
					{
						flag = 1;
					}
				}
				if (flag == 0)
				{
					s.push(temp);
					Q.push_back(temp);
				}
			}
			trans_edge.push(temp); // 将转换结果进队列
		}
	}

	int i, j;
	// 初始化点数、边数、转换字符集
	dfa.nodeCount = Q.size();
	dfa.edgeCount = trans_edge.size();
	dfa.transchar = nfa.transchar;

	// 初始化节点名称
	dfa.vertex.resize(dfa.nodeCount);

	for (i = dfa.nodeCount - 1, j = 65; i >= 0; i--, j--)
	{
		dfa.vertex[i] = char(j + dfa.nodeCount - 1);
	}

	// 初始化二维矩阵
	dfa.transet.resize(dfa.nodeCount);
	for (i = 0; i < dfa.nodeCount; i++)
	{
		dfa.transet[i].resize(dfa.nodeCount);
	}
	for (i = 0; i < dfa.nodeCount; i++)
	{
		for (j = 0; j < dfa.nodeCount; j++)
		{
			dfa.transet[i][j] = '$';
		}
	}

	// 从队列中给矩阵赋值
	i = 0;
	while (!trans_edge.empty())
	{
		for (auto trans_iter = dfa.transchar.begin(); trans_iter != dfa.transchar.end(); trans_iter++)
		{

			set<char> temp = trans_edge.front();
			trans_edge.pop();
			if (temp.size() == 1 && *temp.begin() == '$')
			{
				// do nothing
			}
			else
			{
				for (j = 0; j < Q.size(); j++)
				{
					if (equal_set(temp, Q[j]))
					{
						break;
					}
				}
				if (dfa.transet[i][j] == "$")
				{
					dfa.transet[i][j] = "";
				}
				dfa.transet[i][j] += *trans_iter;
			}
		}
		i++;
	}

	// 开始节点为第一个节点
	node start;
	start.Name = dfa.vertex[0];
	dfa.StartNode = start;

	// 构造终止节点集
	for (i = 0; i < Q.size(); i++)
	{
		set<char>::iterator temp_iter;
		temp_iter = Q[i].find(nfa.EndNodeSet[0].Name);
		if (temp_iter != Q[i].end())
		{
			node end;
			end.Name = dfa.vertex[i];
			dfa.EndNodeSet.push_back(end);
			dfa.EndSet.insert(end.Name);
		}
	}

	return dfa;
}
