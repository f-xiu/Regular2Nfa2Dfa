
#include"PublicNfa.h"
#include "DFAmini.h"
#include <stack>
#include <vector>
#include <queue>
#include<unordered_map>
#include<algorithm>
using namespace std;


Matrix DFA_minimize(const Matrix& dfa, std::unordered_map<int, string>& map)
{
	vector<set<char>> groups;
	// 初始化groups,分成终止集和非终止集
	groups = init_groups(dfa);

	// 循环进行分组
	groups = grouping(dfa, groups);

	//对长度为1的适当排序
	sort(groups.begin(), groups.end(), [](set<char>a, set<char>b) {
		int two = 0;
		if (a.size() == 1 && b.size() == 1) {
			two = *(a.begin()) < *(b.begin());
		}
		return two;
		});


	// groups中获得了新节点数，先初始化dfa_min的节点信息
	int i, j;
	Matrix dfa_min;
	dfa_min.nodeCount = groups.size();
	dfa_min.edgeCount = 0;
	dfa_min.transchar = dfa.transchar;
	dfa_min.vertex.resize(dfa_min.nodeCount);
	for (int i = 0;i < groups.size();i++) {
		string tm = "";
		if (groups[i].size() == 1)dfa_min.vertex[i] = *(groups[i].begin());
		else dfa_min.vertex[i] = 'A' + i;
		tm += dfa_min.vertex[i];
		map.insert({ i,tm });
	}

	int endNum = 0;
	for (i = 0; i < groups.size(); i++)
	{
		if (groups[i].find(dfa.StartNode.Name) != groups[i].end())
		{
			dfa_min.StartNode.Name = dfa_min.vertex[i];
			if (map.find(i) != map.end()) {
				map[i] = "X";
			}
			else map.insert({ i,"X" });
		}
		//遍历每一个终结符，是否在集合中，重名为Y，Y1。。。
		for (auto end_iter = dfa.EndNodeSet.begin(); end_iter != dfa.EndNodeSet.end(); end_iter++)
		{
			if (groups[i].find((*end_iter).Name) != groups[i].end())
			{
				node newEnd;
				newEnd.Name = dfa_min.vertex[i];
				dfa_min.EndNodeSet.push_back(newEnd);
				dfa_min.EndSet.insert(newEnd.Name);
				string tm = "Y";
				if (endNum > 0) {
					tm += (char)('0' + endNum);
				}
				if (map.find(i) != map.end()) {
					map[i] = tm;
				}
				else map.insert({ i,tm });
				endNum++;
				break;
			}
		}

	}

	// 初始化边集
	dfa_min.transet.resize(dfa_min.nodeCount);
	for (i = 0; i < dfa_min.nodeCount; i++)
	{
		dfa_min.transet[i].resize(dfa_min.nodeCount);
	}
	for (i = 0; i < dfa_min.nodeCount; i++)
	{
		for (j = 0; j < dfa_min.nodeCount; j++)
		{
			dfa_min.transet[i][j] = "$";
		}
	}

	// 求新的边集
	for (i = 0; i < groups.size(); i++)
	{
		for (auto trans_iter = dfa.transchar.begin(); trans_iter != dfa.transchar.end(); trans_iter++)
		{
			set<char> trans_result;
			for (auto set_iter = groups[i].begin(); set_iter != groups[i].end(); set_iter++)
			{
				trans_result = union_set(trans_result, (transfer_result(dfa, *set_iter, *trans_iter)));
			}
			if (trans_result.size() != 0)
			{
				for (j = 0; j < groups.size(); j++)
				{
					if (sub_set(groups[j], trans_result))
					{
						break;
					}
				}
				if (dfa_min.transet[i][j] == "$")
				{
					dfa_min.transet[i][j] = "";
				}
				dfa_min.transet[i][j] += *trans_iter;
				dfa_min.edgeCount++;
			}
		}
	}

	return dfa_min;
}

// 初始化一个将节点分为“终结集”和“非终结集”的容器
vector<set<char>> init_groups(const Matrix& dfa)
{
	vector<set<char>> groups;

	// 首先划分出终结集
	set<char> endgroup;
	for (auto endnode_iter = dfa.EndNodeSet.begin(); endnode_iter != dfa.EndNodeSet.end(); endnode_iter++)
	{
		endgroup.insert((*endnode_iter).Name);
	}
	groups.push_back(endgroup);

	// 然后划分出非终结集
	set<char> not_endgroup;
	int i;
	for (i = 0; i < dfa.nodeCount; i++)
	{
		if (endgroup.find(dfa.vertex[i]) == endgroup.end())
		{
			not_endgroup.insert(dfa.vertex[i]);
		}
	}
	if (not_endgroup.size() != 0) // 有可能所有节点都是终止结点，所以仅当非终结集不为空时加入
	{
		groups.push_back(not_endgroup);
	}

	return groups;
}

// 从“终结集”和“非终结集”开始，进行分组
vector<set<char>> grouping(const Matrix& dfa, const vector<set<char>>& in_groups)
{
	vector<set<char>> groups(in_groups);

	// 如果所有节点都是终止结点那就不需要做分组，否则进行循环分组
	int i, j, k, m;
	if (groups.size() != 1)
	{
		int c = groups.size(), p = 0; // 计数器，当groups中不再加入新组时(c==p)循环结束
		while (c != p)
		{
			p = c;
			int flag = 0; // 分组标志，0表示不需要分组，1表示需要

			// 对groups中每个集合，检查是否需要分组
			for (i = 0; i < groups.size(); i++)
			{
				// 对每个转换字符，求转换结果，检查是否需要分组
				set<char>::iterator transchar_iter;
				for (transchar_iter = dfa.transchar.begin(); transchar_iter != dfa.transchar.end(); transchar_iter++)
				{
					// 求转换结果
					vector<char> trans_result;
					for (auto set_iter = groups[i].begin(); set_iter != groups[i].end(); set_iter++)
					{
						// 由于dfa中节点经过一个转换字符只能到一个终点，因此直接取begin()即可
						// 不过还要考虑空集的问题
						set<char> temp = transfer_result(dfa, *set_iter, *transchar_iter);
						if (temp.size() == 0)
						{
							temp.insert('$');
						}
						trans_result.push_back(*(temp.begin()));
					}

					// 检查是否需要分组
					vector<int> grouping;
					for (j = 0; j < trans_result.size(); j++)
					{
						if (trans_result[j] == '$')
						{
							grouping.push_back(-1);
						}
						else
						{
							for (k = 0; k < groups.size(); k++)
							{
								if (groups[k].find(trans_result[j]) != groups[k].end())
								{
									grouping.push_back(k);
									break;
								}
							}
						}
					}

					// 如果来自不止一个组，则需要分组
					set<int> group_num(grouping.begin(), grouping.end());
					if (group_num.size() != 1)
					{
						flag = 1;

						for (auto grouping_iter = group_num.begin(); grouping_iter != group_num.end(); grouping_iter++)
						{
							set<char> new_group;
							set<char>::iterator set_iter;
							for (set_iter = groups[i].begin(), m = 0; set_iter != groups[i].end(); set_iter++, m++)
							{
								if (grouping[m] == *grouping_iter)
								{
									new_group.insert(*set_iter);
								}
							}
							groups.push_back(new_group);
						}

						groups.erase(groups.begin() + i);
						break;
					}
				}

				if (flag == 1)
				{
					break;
				}
			}
			c = groups.size();
		}
	}
	return groups;
}

// 判断s2是否是s1子集
bool sub_set(const set<char>& s1, const set<char>& s2)
{
	if (s2.size() > s1.size())
		return false;

	for (auto set_iter = s2.begin(); set_iter != s2.end(); set_iter++)
	{
		if (s1.find(*set_iter) == s1.end())
		{
			return false;
		}
	}
	return true;
}
