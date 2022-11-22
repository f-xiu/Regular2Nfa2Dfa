#include "Nfa2Dfa.h"
#include "PublicNfa.h"
#include <stack>
#include <vector>
#include <queue>
#include<set>
using namespace std;

// ��NFA_matrixת��ΪDFA_matrix
Matrix NFA2DFA(const Matrix& nfa)
{
	Matrix dfa;
	vector<set<char>> Q;

	// ����Ӧ��Q������Ԫ���������ٱ仯ʱֹͣѭ��������һ�����нṹ��ʵ��
	queue<set<char>> s;
	// ��ʼʱ����ʼ�ڵ�Ħűհ������
	s.push(e_closure(nfa, nfa.StartNode.Name));
	Q.push_back(e_closure(nfa, nfa.StartNode.Name));

	// ��������ܱ���ת�������������һ�����������
	queue<set<char>> trans_edge;

	// �Ӽ�����ȡ�½ڵ�
	while (!s.empty())
	{
		set<char> top = s.front(); // ȡ�����׼���
		s.pop();

		// ��ÿһ��ת���ַ�����ת�����
		set<char>::iterator transchar_iter;
		for (transchar_iter = nfa.transchar.begin(); transchar_iter != nfa.transchar.end(); transchar_iter++)
		{

			set<char> temp;
			int flag = 0;

			// ������ȡ�������е�ÿһ��Ԫ�أ�����ת������Ħ�-�հ������ϲ���temp��
			set<char>::iterator top_iter;
			for (top_iter = top.begin(); top_iter != top.end(); top_iter++)
			{
				temp = union_set(temp, transfer_result(nfa, *top_iter, *transchar_iter));
			}

			if (temp.size() == 0)
			{
				//���������Ϊ�գ������$��ʾ
				temp.insert('$');
			}
			else
			{
				// ����Q�����Ƿ�������temp��ͬ�ļ��ϣ�û�������ջ��
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
			trans_edge.push(temp); // ��ת�����������
		}
	}

	int i, j;
	// ��ʼ��������������ת���ַ���
	dfa.nodeCount = Q.size();
	dfa.edgeCount = trans_edge.size();
	dfa.transchar = nfa.transchar;

	// ��ʼ���ڵ�����
	dfa.vertex.resize(dfa.nodeCount);

	for (i = dfa.nodeCount - 1, j = 65; i >= 0; i--, j--)
	{
		dfa.vertex[i] = char(j + dfa.nodeCount - 1);
	}

	// ��ʼ����ά����
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

	// �Ӷ����и�����ֵ
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

	// ��ʼ�ڵ�Ϊ��һ���ڵ�
	node start;
	start.Name = dfa.vertex[0];
	dfa.StartNode = start;

	// ������ֹ�ڵ㼯
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
