#include"Regular2NFA.h"
#include<iostream>
#include <stack>
#include <vector>
#include <queue>
using namespace std;

// ��������ʽ
string input_regex()
{
	string regex;
	cout << "Please input regex:  (Valid operator: () * |; Charset: a~z A~Z)\n";
	do
	{
		cin >> regex;
	} while (!check_legal(regex));

	return regex;
}

// ������������ʽ�Ƿ�Ϸ�
bool check_legal(const string& regex)
{
	if (check_character(regex) && check_parenthesis(regex))
	{
		return true;
	}
	return false;
}

// �������ʽ�Ƿ��зǷ��ַ�
bool check_character(const string& regex)
{
	int i;
	for (i = 0; i < regex.length(); i++)
	{
		if (is_letter(regex[i]))
		{
			// cout << "���ַ�" << endl;
		}
		else if (is_operator(regex[i]))
		{
			// cout << "�Ƿ���" << endl;
		}
		else
		{
			cerr << "Invalid character in input. Please retry!\n";
			return false;
		}
	}
	return true;
}

// �Ƿ��ǺϷ��ַ�
bool is_letter(char c)
{
	if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
	{
		return true;
	}
	return false;
}

// �Ƿ��������
bool is_operator(char c)
{
	if (c == '(' || c == ')' || c == '*' || c == '|' || c == '+')
	{
		return true;
	}
	return false;
}

// �������ʽ�����űպ�
bool check_parenthesis(const string& regex)
{
	stack<int> s;
	int i;
	for (i = 0; i < regex.length(); i++)
	{
		if (regex[i] == '(')
		{
			s.push(1);
		}
		else if (regex[i] == ')')
		{
			if (s.empty())
			{
				std::cerr << "Redundant right parentheses. Please retry!\n";
				return false;
			}
			else
				s.pop();
		}
	}
	if (!s.empty())
	{
		cerr << "Redundant left parentheses. Please retry!\n";
		return false;
	}
	return true;
}

/*
�������ȼ�����򣺣�1���������ڣ��������⣻��2�����ȼ��ɸߵ��ͣ��հ���|��+����3��ͬ����������ҡ�
���ȼ���
	 #	(	*	|	+	)
isp  0	1	7	5	3	8
icp	 0	8	6	4	2	1

isp: in stack priority��ջ�����ȼ�
icp��in coming priority����ջ���ȼ�
*/

/*********************��������ȼ���ϵ��********************/
/*
	c1\c2	(	*	|	+	)	#

	(		<	<	<	<	=	>

	*		<	>	>	>	>	>

	|		<	<	>	>	>	>

	)       =	>	>	>	>	>

	+		<	<	<	>	>	>

	#		<	<	<	<	<	=
*/

// �ϱ���˼Ϊ��c1 _(<|>|=) c2

/***********************************************************/

// ������
// ��׺��a(a|b)*((ab)|c)*
// ����׺���ָ�����a+(a|b)*+((a+b)|c)*�������Ϊ��׺���ʽ��ͬʱҲ������(ab)���������⣩
// ��׺��aab|*+ab+c|*+

// Ϊ��׺���ʽ��ӷָ���(��+��)
string add_separation(const string& regex)
{
	std::string result;
	char c, p; // current character, previous character
	for (int i = 1; i < regex.length(); i++)
	{
		p = regex[i - 1];
		c = regex[i];
		result.push_back(p);
		// ���濪ʼ�������ӡ�+��
		// 1����c����ĸ��p����'('��'|'��Ҫ���
		if (p != '(' && p != '|' && is_letter(c))
		{
			result.push_back('+');
		}
		// 2����c��'(',p����'|'��'(',ҲҪ��
		else if (c == '(' && p != '|' && p != '(')
		{
			result.push_back('+');
		}
	}
	result.push_back(c); // �����һ���ַ�����
	return result;
}

// ��׺���ʽת��׺���ʽ
string to_postfix(string& regex)
{
	regex += "#";					 // ����ת����׺���ʽ������ֹ��'#'
	auto ch = regex.begin();
	char op;

	string result;

	stack<char> s;
	s.push('#'); // ��ʼ����ջ
	while (!s.empty())
	{
		if (is_letter(*ch))
		{
			result.push_back(*ch);
			ch++;
		}
		else
		{
			op = s.top();
			if (isp(op) < icp(*ch)) // ���ջ�����ȼ�С����ջ���ȼ������ջ
			{
				s.push(*ch);
				ch++;
			}
			else if (isp(op) > icp(*ch)) // ���ջ�����ȼ�������ջ���ȼ�������ջ��������
			{
				result.push_back(op);
				s.pop();
			}
			else // ���ջ�����ȼ�������ջ���ȼ�������ջ������������
			{
				s.pop();
				// ���ȼ���ȵ����ֻ��'#'-'#'��'('-')'����ֻ��ջ��Ϊ'('����')'ʱ�Ų�����ֹ״̬
				if (op == '(')
				{
					ch++;
				}
			}
		}
	}
	return result;
}

// isp
int isp(char c)
{
	switch (c)
	{
	case '#':
		return 0;
	case '(':
		return 1;
	case '*':
		return 7;
	case '|':
		return 5;
	case '+':
		return 3;
	case ')':
		return 8;
	default:
		break;
	}
	cerr << "isp error\n";
	return -1;
}

// icp
int icp(char c)
{
	switch (c)
	{
	case '#':
		return 0;
	case '(':
		return 8;
	case '*':
		return 6;
	case '|':
		return 4;
	case '+':
		return 2;
	case ')':
		return 1;
	}
	cerr << "icp error!\n";
	return -1;
}

// ����Thompson�㷨��������NFA��
// �Բ�ͬ�Ĳ���������Ҫ��ͬ�����������������a|b��ab��a*��a������ֱ��ú���ʵ��

char STATE_NAME = 65; // ����NFA�нڵ�����ƣ���A��ʼ

cell regex_to_nfa(const string& regex)
{
	int i, flag = 0;
	char ch;
	set<char> transchar; // ��¼ת���ַ�
	cell NFA, Cell, Left, Right;
	// NFA�����ս��
	// Cell����ʱ����
	// Left���������
	// Right���Ҳ�����
	stack<cell> s;
	for (i = 0; i < regex.length(); i++)
	{
		ch = regex[i];
		switch (ch)
		{
		case '|':
		{
			Right = s.top();
			s.pop();
			Left = s.top();
			s.pop();
			Cell = unite_cell(Left, Right);
			s.push(Cell);
			break;
		}
		case '*':
		{
			Left = s.top();
			s.pop();
			Cell = loop_cell(Left);
			s.push(Cell);
			break;
		}
		case '+':
		{
			Right = s.top();
			s.pop();
			Left = s.top();
			s.pop();
			Cell = join_cell(Left, Right);
			s.push(Cell);
			break;
		}
		default:
			Cell = init_cell(ch);
			s.push(Cell);
			if (ch != '#')
				transchar.insert(ch); // set�����ȼ���Ƿ���ڣ�ֻ�е�������ʱ�Ż����
			break;
		}
	}
	NFA = s.top();
	s.pop();
	NFA.transchar = transchar;
	STATE_NAME = 65; // �����������ã������ٴε���
	NFA.vertex = "";
	set<char>nodeset;
	for (int i = 0;i < NFA.EdgeCount;i++) {
		char start = NFA.EdgeSet[i].Start.Name;
		char end = NFA.EdgeSet[i].End.Name;
		if (nodeset.find(start)==nodeset.end()) {
			nodeset.insert(start);
			NFA.vertex += start;
		}
		if (nodeset.find(end) == nodeset.end()) {
			nodeset.insert(end);
			NFA.vertex += end;
		}
	}
	NFA.NodeCount = NFA.vertex.size();
	return NFA;
}

// ��ʼ���ڵ�
cell init_cell(char ch)
{
	edge newEdge;
	newEdge.Start.Name = STATE_NAME++;
	newEdge.End.Name = STATE_NAME++;
	newEdge.transchar = ch;

	cell newCell;
	newCell.EdgeCount = 0;
	newCell.EdgeSet.push_back(newEdge);
	newCell.EdgeCount++;
	newCell.StartNode = newEdge.Start;
	newCell.EndNodeSet.push_back(newEdge.End);
	return newCell;
}

// ������(a|b)
// һ����Ҫ��������ڵ㣬������
// �¿�ʼ�ڵ�ָ��Left/Right��ʼ�ڵ㣬Left/Right��ֹ�ڵ�ָ������ֹ�ڵ�
cell unite_cell(const cell& Left, const cell& Right)
{
	cell newCell;
	newCell.EdgeCount = 0;
	newCell.StartNode.Name = STATE_NAME++;
	node newEnd;
	newEnd.Name = STATE_NAME++;
	newCell.EndNodeSet.push_back(newEnd);

	edge edge1, edge2, edge3, edge4;
	// �¿�ʼ�ڵ�ָ��Left��ʼ�ڵ�
	edge1.Start = newCell.StartNode;
	edge1.End = Left.StartNode;
	edge1.transchar = '#';

	// �¿�ʼ�ڵ�ָ��Right��ʼ�ڵ�
	edge2.Start = newCell.StartNode;
	edge2.End = Right.StartNode;
	edge2.transchar = '#';

	// Left��ֹ�ڵ�ָ������ֹ�ڵ�
	edge3.Start = Left.EndNodeSet[0];
	edge3.End = newCell.EndNodeSet[0];
	edge3.transchar = '#';

	// Left��ֹ�ڵ�ָ������ֹ�ڵ�
	edge4.Start = Right.EndNodeSet[0];
	edge4.End = newCell.EndNodeSet[0];
	edge4.transchar = '#';

	// ��Left��Right��EdgeSet���Ƶ�NewCell
	copy_cell(newCell, Left);
	copy_cell(newCell, Right);

	// �������ı߼���
	newCell.EdgeSet.push_back(edge1);
	newCell.EdgeSet.push_back(edge2);
	newCell.EdgeSet.push_back(edge3);
	newCell.EdgeSet.push_back(edge4);
	newCell.EdgeCount += 4;

	return newCell;
}

// ������(a+b)
// Left�Ľ���״̬��Right�Ŀ�ʼ״̬�ϲ�
cell join_cell(const cell& in_Left, const cell& in_Right)
{
	// ������Ҫ���ĸ��ڵ�ϲ�������(A-a-B��C-b-D �� A-a-B-b-D)
	// ��Left�Ľ���״̬��Right�Ŀ�ʼ״̬�ϲ�������Right�������ı߸��Ƹ�Left����󷵻�Left
	cell result(in_Left), Right(in_Right);

	int i;
	// �������ϲ�����
	for (i = 0; i < Right.EdgeCount; i++)
	{
		// Right�Ŀ�ʼ״̬��Left�Ľ���״̬�ϲ�
		if (Right.EdgeSet[i].Start.Name == Right.StartNode.Name)
		{
			Right.EdgeSet[i].Start = in_Left.EndNodeSet[0];
			// STATE_NAME--;
		}
		else if (Right.EdgeSet[i].End.Name == Right.StartNode.Name)
		{
			Right.EdgeSet[i].End = in_Left.EndNodeSet[0];
			// STATE_NAME--;
		}

		// ��Right����ֹ���ǰ��
		if (Right.EdgeSet[i].End.Name == Right.EndNodeSet[0].Name)
		{
			Right.EdgeSet[i].End.Name -= 1;
		}
		if (Right.EdgeSet[i].Start.Name == Right.EndNodeSet[0].Name)
		{
			Right.EdgeSet[i].Start.Name -= 1;
		}
	}
	Right.StartNode = in_Left.EndNodeSet[0];
	Right.EndNodeSet[0].Name -= 1;
	STATE_NAME--;

	// Ȼ�����߸��ƵĲ���
	copy_cell(result, Right);

	result.EndNodeSet = Right.EndNodeSet;
	return result;
}

// �հ�����(a*)
// һ����Ҫ��������ڵ㣬�����ߡ�
// �¿�ʼ�ڵ�ָ��ԭ��ʼ��㣬ԭ��ֹ���ָ������ֹ�ڵ㣬ԭ��ֹ�ڵ�ָ��ԭ��ʼ�ڵ�ıߣ��¿�ʼ���ָ������ֹ�ڵ�
cell loop_cell(const cell& Left)
{
	cell newCell;
	newCell.EdgeCount = 0;
	newCell.StartNode.Name = STATE_NAME++;
	node newEnd;
	newEnd.Name = STATE_NAME++;
	newCell.EndNodeSet.push_back(newEnd);

	edge edge1, edge2, edge3, edge4;
	// �¿�ʼ�ڵ�ָ��ԭ��ʼ���
	edge1.Start = newCell.StartNode;
	edge1.End = Left.StartNode;
	edge1.transchar = '#';

	// ԭ��ֹ���ָ������ֹ�ڵ�
	edge2.Start = Left.EndNodeSet[0];
	edge2.End = newCell.EndNodeSet[0];
	edge2.transchar = '#';

	// ԭ��ֹ�ڵ�ָ��ԭ��ʼ�ڵ�ı�
	edge3.Start = Left.EndNodeSet[0];
	edge3.End = Left.StartNode;
	edge3.transchar = '#';

	// �¿�ʼ���ָ������ֹ�ڵ�
	edge4.Start = newCell.StartNode;
	edge4.End = newCell.EndNodeSet[0];
	edge4.transchar = '#';

	// ��ԭ���ı߸��ƹ���
	copy_cell(newCell, Left);
	// ���������������
	newCell.EdgeSet.push_back(edge1);
	newCell.EdgeSet.push_back(edge2);
	newCell.EdgeSet.push_back(edge3);
	newCell.EdgeSet.push_back(edge4);
	newCell.EdgeCount += 4;

	return newCell;
}

// ��c2�еı߿�����c1
void copy_cell(cell& c1, const cell& c2)
{
	int i;
	c1.EdgeSet.resize(c1.EdgeCount + c2.EdgeCount);
	for (i = 0; i < c2.EdgeCount; i++)
	{
		c1.EdgeSet[c1.EdgeCount + i] = c2.EdgeSet[i];
	}
	c1.EdgeCount += c2.EdgeCount;
}

// �������cell