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


grup getNFA(string text)
{
	stack<grup> s;
	for (int i = 0; i < int(text.length()); i++)
	{
		if ((text[i] <= 'z' && text[i] >= 'a') || (text[i] <= 'Z' && text[i] >= 'A') || (text[i] <= '9' && text[i] >= '0'))
		{
			grup tempG;
			Edge tempE;
			tempE.accept = text[i];
			tempE.start = 0;
			tempE.end = 1;
			tempG.Edges.push_back(tempE);
			s.push(tempG);
		}
		else
		{
			if (text[i] == '+')
			{
				grup tempG2 = s.top();
				s.pop();
				grup tempG1 = s.top();
				s.pop();
				//图和图合成
				grup tempG = tempG1;
				tempG.stateCount = tempG1.stateCount + tempG2.stateCount - 1;
				tempG.StartState = 0;
				tempG.EndState = tempG.stateCount - 1;
				//获取后一条边
				for (int i = 0; i < int(tempG2.Edges.size()); i++)
				{
					Edge e2 = tempG2.Edges[i];
					e2.start += tempG1.stateCount - 1;
					e2.end += tempG1.stateCount - 1;
					tempG.Edges.push_back(e2);
				}
				//合成图压栈
				s.push(tempG);
			}
			else if (text[i] == '*')
			{
				grup tempG2 = s.top();
				s.pop();
				grup tempG;
				if (tempG2.StartState != tempG2.EndState)
				{
					tempG.stateCount = tempG2.stateCount + 1;
				}
				else
				{
					tempG.stateCount = tempG2.stateCount + 2;
				}
				tempG.EndState = tempG.stateCount - 1;
				//图内的边编号加1
				for (int i = 0; i < int(tempG2.Edges.size()); i++)
				{
					Edge e2 = tempG2.Edges[i];
					if (e2.end == tempG2.EndState)
					{
						e2.start += 1;
						e2.end = e2.start;
					}
					else
					{
						e2.start += 1;
						e2.end += 1;
					}
					tempG.Edges.push_back(e2);
				}
				//添加两条边
				Edge e1;
				e1.accept = '~';
				e1.start = 0;
				e1.end = 1;
				Edge e2;
				e2.accept = '~';
				e2.start = tempG.EndState - 1;
				e2.end = e2.start + 1;
				tempG.Edges.push_back(e1);
				tempG.Edges.push_back(e2);
				//合成图压栈
				s.push(tempG);
			}
			else if (text[i] == '|')
			{
				grup tempG2 = s.top();
				s.pop();
				grup tempG1 = s.top();
				s.pop();
				grup tempG;
				tempG.StartState = 0;
				tempG.EndState = 0;
				tempG.stateCount = tempG1.stateCount + tempG2.stateCount - 3;
				for (int i = 0; i < int(tempG1.Edges.size()); i++)
				{
					Edge e1 = tempG1.Edges[i];
					if (e1.end == tempG1.EndState)
					{
						e1.end = 0;
					}
					tempG.Edges.push_back(e1);
				}
				for (int i = 0; i < int(tempG2.Edges.size()); i++)
				{
					Edge e2 = tempG2.Edges[i];
					if (e2.end == tempG2.EndState)
					{
						e2.end = 0;
					}
					tempG.Edges.push_back(e2);
				}
				//合成图压栈
				s.push(tempG);
			}
		}
	}
	return s.top();
}

void printNFA(grup out)
{
	ofstream fout;
	fout.open("./nfa.txt");

	fout << "X ";
	for (int j = 0; j < int(out.Edges.size()); j++)
	{
		Edge eX = out.Edges[j];
		if (eX.start == 0)
		{
			fout << "X-" << eX.accept << "->";
			if (eX.end == out.stateCount - 1)
			{
				fout << "Y ";
			}
			else
			{
				fout << eX.end - 1 << " ";
			}
		}
	}
	fout << endl;
	fout << "Y ";
	for (int j = 0; j < int(out.Edges.size()); j++)
	{
		Edge eX = out.Edges[j];
		if (eX.start == out.stateCount - 1)
		{
			fout << "Y-" << eX.accept << "->";
			if (eX.end == out.stateCount - 1)
			{
				fout << "Y ";
			}
			else
			{
				fout << eX.end - 1 << " ";
			}
		}
	}
	fout << endl;
	for (int i = 1; i < out.stateCount - 1; i++)
	{
		fout << i - 1 << " ";
		for (int j = 0; j < int(out.Edges.size()); j++)
		{
			Edge e = out.Edges[j];
			if (e.start == i && e.accept == '~')
			{
				fout << e.start - 1 << "-" << e.accept << "->";
				if (e.end == out.stateCount - 1)
				{
					fout << "Y ";
				}
				else
				{
					fout << e.end - 1 << " ";
				}
			}
		}
		for (int j = 0; j < int(out.Edges.size()); j++)
		{
			Edge e = out.Edges[j];
			if (e.start == i && e.accept != '~')
			{
				fout << e.start - 1 << "-" << e.accept << "->";
				if (e.end == out.stateCount - 1)
				{
					fout << "Y ";
				}
				else
				{
					fout << e.end - 1 << " ";
				}
			}
		}
		fout << endl;
	}
	fout.close();
}