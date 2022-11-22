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
				//ͼ��ͼ�ϳ�
				grup tempG = tempG1;
				tempG.stateCount = tempG1.stateCount + tempG2.stateCount - 1;
				tempG.StartState = 0;
				tempG.EndState = tempG.stateCount - 1;
				//��ȡ��һ����
				for (int i = 0; i < int(tempG2.Edges.size()); i++)
				{
					Edge e2 = tempG2.Edges[i];
					e2.start += tempG1.stateCount - 1;
					e2.end += tempG1.stateCount - 1;
					tempG.Edges.push_back(e2);
				}
				//�ϳ�ͼѹջ
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
				//ͼ�ڵı߱�ż�1
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
				//���������
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
				//�ϳ�ͼѹջ
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
				//�ϳ�ͼѹջ
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