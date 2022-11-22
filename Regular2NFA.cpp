#include"Regular2NFA.h"
#include<iostream>
#include <stack>
#include <vector>
#include <queue>
using namespace std;

// 输入正规式
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

// 检查输入的正规式是否合法
bool check_legal(const string& regex)
{
	if (check_character(regex) && check_parenthesis(regex))
	{
		return true;
	}
	return false;
}

// 检查正规式是否有非法字符
bool check_character(const string& regex)
{
	int i;
	for (i = 0; i < regex.length(); i++)
	{
		if (is_letter(regex[i]))
		{
			// cout << "是字符" << endl;
		}
		else if (is_operator(regex[i]))
		{
			// cout << "是符号" << endl;
		}
		else
		{
			cerr << "Invalid character in input. Please retry!\n";
			return false;
		}
	}
	return true;
}

// 是否是合法字符
bool is_letter(char c)
{
	if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
	{
		return true;
	}
	return false;
}

// 是否是运算符
bool is_operator(char c)
{
	if (c == '(' || c == ')' || c == '*' || c == '|' || c == '+')
	{
		return true;
	}
	return false;
}

// 检查正规式的括号闭合
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
构造优先级表规则：（1）先括号内，再括号外；（2）优先级由高到低：闭包、|、+；（3）同级别，先左后右。
优先级表：
	 #	(	*	|	+	)
isp  0	1	7	5	3	8
icp	 0	8	6	4	2	1

isp: in stack priority，栈内优先级
icp：in coming priority，入栈优先级
*/

/*********************运算符优先级关系表********************/
/*
	c1\c2	(	*	|	+	)	#

	(		<	<	<	<	=	>

	*		<	>	>	>	>	>

	|		<	<	>	>	>	>

	)       =	>	>	>	>	>

	+		<	<	<	>	>	>

	#		<	<	<	<	<	=
*/

// 上表意思为：c1 _(<|>|=) c2

/***********************************************************/

// 举例：
// 中缀：a(a|b)*((ab)|c)*
// 对中缀做分隔处理：a+(a|b)*+((a+b)|c)*，方便改为后缀表达式（同时也消解了(ab)的歧义问题）
// 后缀：aab|*+ab+c|*+

// 为中缀表达式添加分隔符(‘+’)
string add_separation(const string& regex)
{
	std::string result;
	char c, p; // current character, previous character
	for (int i = 1; i < regex.length(); i++)
	{
		p = regex[i - 1];
		c = regex[i];
		result.push_back(p);
		// 下面开始分情况添加‘+’
		// 1、若c是字母、p不是'('、'|'都要添加
		if (p != '(' && p != '|' && is_letter(c))
		{
			result.push_back('+');
		}
		// 2、若c是'(',p不是'|'、'(',也要加
		else if (c == '(' && p != '|' && p != '(')
		{
			result.push_back('+');
		}
	}
	result.push_back(c); // 把最后一个字符加上
	return result;
}

// 中缀表达式转后缀表达式
string to_postfix(string& regex)
{
	regex += "#";					 // 给待转换中缀表达式加入终止符'#'
	auto ch = regex.begin();
	char op;

	string result;

	stack<char> s;
	s.push('#'); // 开始符入栈
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
			if (isp(op) < icp(*ch)) // 如果栈内优先级小于入栈优先级，则进栈
			{
				s.push(*ch);
				ch++;
			}
			else if (isp(op) > icp(*ch)) // 如果栈内优先级大于入栈优先级，则退栈并加入结果
			{
				result.push_back(op);
				s.pop();
			}
			else // 如果栈内优先级等于入栈优先级，则退栈，但不加入结果
			{
				s.pop();
				// 优先级相等的情况只有'#'-'#'，'('-')'，而只有栈顶为'('读入')'时才不是终止状态
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

// 采用Thompson算法构建整个NFA，
// 对不同的操作符，需要不同数量的运算数，因此a|b、ab、a*、a操作需分别用函数实现

char STATE_NAME = 65; // 定义NFA中节点的名称，从A开始

cell regex_to_nfa(const string& regex)
{
	int i, flag = 0;
	char ch;
	set<char> transchar; // 记录转换字符
	cell NFA, Cell, Left, Right;
	// NFA：最终结果
	// Cell：临时变量
	// Left：左操作符
	// Right：右操作符
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
				transchar.insert(ch); // set会首先检测是否存在，只有当不存在时才会插入
			break;
		}
	}
	NFA = s.top();
	s.pop();
	NFA.transchar = transchar;
	STATE_NAME = 65; // 将计数器重置，方便再次调用
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

// 初始化节点
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

// 或运算(a|b)
// 一共需要添加两个节点，四条边
// 新开始节点指向Left/Right开始节点，Left/Right终止节点指向新终止节点
cell unite_cell(const cell& Left, const cell& Right)
{
	cell newCell;
	newCell.EdgeCount = 0;
	newCell.StartNode.Name = STATE_NAME++;
	node newEnd;
	newEnd.Name = STATE_NAME++;
	newCell.EndNodeSet.push_back(newEnd);

	edge edge1, edge2, edge3, edge4;
	// 新开始节点指向Left开始节点
	edge1.Start = newCell.StartNode;
	edge1.End = Left.StartNode;
	edge1.transchar = '#';

	// 新开始节点指向Right开始节点
	edge2.Start = newCell.StartNode;
	edge2.End = Right.StartNode;
	edge2.transchar = '#';

	// Left终止节点指向新终止节点
	edge3.Start = Left.EndNodeSet[0];
	edge3.End = newCell.EndNodeSet[0];
	edge3.transchar = '#';

	// Left终止节点指向新终止节点
	edge4.Start = Right.EndNodeSet[0];
	edge4.End = newCell.EndNodeSet[0];
	edge4.transchar = '#';

	// 将Left和Right的EdgeSet复制到NewCell
	copy_cell(newCell, Left);
	copy_cell(newCell, Right);

	// 将新增的边加入
	newCell.EdgeSet.push_back(edge1);
	newCell.EdgeSet.push_back(edge2);
	newCell.EdgeSet.push_back(edge3);
	newCell.EdgeSet.push_back(edge4);
	newCell.EdgeCount += 4;

	return newCell;
}

// 与运算(a+b)
// Left的结束状态与Right的开始状态合并
cell join_cell(const cell& in_Left, const cell& in_Right)
{
	// 我们需要将四个节点合并成三个(A-a-B，C-b-D → A-a-B-b-D)
	// 即Left的结束状态和Right的开始状态合并，并将Right中其他的边复制给Left，最后返回Left
	cell result(in_Left), Right(in_Right);

	int i;
	// 首先做合并处理
	for (i = 0; i < Right.EdgeCount; i++)
	{
		// Right的开始状态与Left的结束状态合并
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

		// 将Right的终止结点前移
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

	// 然后做边复制的操作
	copy_cell(result, Right);

	result.EndNodeSet = Right.EndNodeSet;
	return result;
}

// 闭包运算(a*)
// 一共需要添加两个节点，四条边。
// 新开始节点指向原开始结点，原终止结点指向新终止节点，原终止节点指向原开始节点的边，新开始结点指向新终止节点
cell loop_cell(const cell& Left)
{
	cell newCell;
	newCell.EdgeCount = 0;
	newCell.StartNode.Name = STATE_NAME++;
	node newEnd;
	newEnd.Name = STATE_NAME++;
	newCell.EndNodeSet.push_back(newEnd);

	edge edge1, edge2, edge3, edge4;
	// 新开始节点指向原开始结点
	edge1.Start = newCell.StartNode;
	edge1.End = Left.StartNode;
	edge1.transchar = '#';

	// 原终止结点指向新终止节点
	edge2.Start = Left.EndNodeSet[0];
	edge2.End = newCell.EndNodeSet[0];
	edge2.transchar = '#';

	// 原终止节点指向原开始节点的边
	edge3.Start = Left.EndNodeSet[0];
	edge3.End = Left.StartNode;
	edge3.transchar = '#';

	// 新开始结点指向新终止节点
	edge4.Start = newCell.StartNode;
	edge4.End = newCell.EndNodeSet[0];
	edge4.transchar = '#';

	// 将原来的边复制过来
	copy_cell(newCell, Left);
	// 添加新增的四条边
	newCell.EdgeSet.push_back(edge1);
	newCell.EdgeSet.push_back(edge2);
	newCell.EdgeSet.push_back(edge3);
	newCell.EdgeSet.push_back(edge4);
	newCell.EdgeCount += 4;

	return newCell;
}

// 将c2中的边拷贝到c1
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

// 重载输出cell