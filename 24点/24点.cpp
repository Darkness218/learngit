// 24点.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <map>
using namespace::std;

class _24 {
private:
	vector<int> number;   //记录随机产生的四个数字
	char *anwter;          //记录玩家的答案
	char *result;         //记录正确答案
	bool error;            //判断表达式是否错误
	bool is_right();       
	void creat();          
	void play();
	queue<char> change(char *expression);         //将中缀表达式转化给后缀表达式
	bool calculate(char *expression);      //计算后缀表达式的值
public:
	_24();
	~_24();
	void start();
};


_24::_24():error(false) {
	number.resize(4);
	for (int i = 0; i < 4; i++) {
		number[i] = 0;
	}
	anwter = new char[15];
	result = new char[15];
}

_24::~_24() {
	delete[] anwter;
	delete[] result;
	number.~vector();
}

void _24::start() {
	int key=0;
	cout << "####################24点游戏:####################"<<endl;
	cout << "\n请选择你的操作:" << endl;
	cout << "\n              1,开始游戏" << endl;
	cout << "              2,游戏规则" << endl;
	cout << "              3,游戏结束" << endl;
	while (key!=1&&key!=2&&key!=3)
	{
		std::cin >> key;
	}
	switch (key)
	{
	case 1:
		system("CLS");
		play();
		break;
	case 2:
		system("CLS");
		cout << "游戏规则:" << endl;
		cout << "    随机出现4个数字，利用加减乘除使4个数字的运算结果为24则成功(一切操作请使用英文输入法)" << endl;
		cout << "    Eg: 出现1 2 3 4,输入1*2*3*4或者(1+2+3)*4都为正确答案" << endl;
		cout << "\n返回开始界面请按0" << endl;
		while (key!=0)
		{
			std::cin >> key;
		}
		system("CLS");
		start();
		break;
	case 3:
		_exit(0);
	default:
		break;
	}
}

//判断产生的四个数字是否能够算出24
bool _24::is_right() {
	bool right=false;
	vector<int> num = number;
	sort(num.begin(), num.end());
	char bracket = '(';
	char *tem = new char[15]();
	char sign[21][3] = 
	{ 
		{'+','+','+'},
	    { '*','+','+' },
	    { '+','*','+' },
	    { '*','*','+' },
	    { '*','+','*' },
	    { '*','*','*' },
	    { '-','+','+' },
	    { '+','-','+' },
	   { '-','*','*' },
	   { '*','-','*' },
	   { '/','*','*' },
	   { '*','/','*' },
	   { '*','+','-' },
	   { '-','*','+' },
	   { '*','-','+' },
	   { '*','+','/' },
	   { '/','*','+' },
	   { '*','/','+' },
	   { '*','-','/' },
	   { '/','*','-' },
	   { '*','/','-' }
	};
	int brackets[5][4] = {
		{-1,-1,-1,-1},
	    {0,4,-1,-1},
	    { -1,2,6,-1 },
	    { -1,2,-1,8 },
	    { 0,4,6,10 }
	};
	
	while (!right&&next_permutation(num.begin(), num.end()))
	{
		
		for (int i = 0; i < 21&&!right; i++) {
			if (((sign[i][0] == '+' || sign[i][0] == '-')&&(sign[i][1] == '+' || sign[i][1] == '-')&&(sign[i][1] == '+' || sign[i][1] == '-'))|| ((sign[i][0] == '*' || sign[i][0] == '/') && (sign[i][1] == '*' || sign[i][1] == '/') && (sign[i][1] == '*' || sign[i][1] == '/'))) {
				int k = 0;
				tem[k++] = num[0] + '0';
				tem[k++] = sign[i][0];
				tem[k++] = num[1] + '0';
				tem[k++] = sign[i][1];
				tem[k++] = num[2] + '0';
				tem[k++] = sign[i][2];
				tem[k++] = num[3] + '0';
				right = calculate(tem);
				error = false;
				if (!right) ZeroMemory(tem, 15);
			}
			else {
				for (int j = 0; j < 5&&!right; j++) {
					int k = 0;
					if (k == brackets[j][0]) {
						tem[k++] = bracket;
						bracket = ')';
					}
					tem[k++] = num[0] + '0';
					tem[k++] = sign[i][0];
					if (k == brackets[j][1]) {
						tem[k++] = bracket;
						bracket = ')';
					}
					tem[k++] = num[1] + '0';
					if (k == brackets[j][1]) {
						tem[k++] = bracket;
						bracket = '(';
					}
					tem[k++] = sign[i][1];
					if (k == brackets[j][2]) {
						tem[k++] = bracket;
						bracket = ')';
					}
					tem[k++] = num[2] + '0';
					if (k == brackets[j][2]) {
						tem[k++] = bracket;
						bracket = '(';
					}
					tem[k++] = sign[i][2];
					tem[k++] = num[3] + '0';
					if (k == brackets[j][3]) {
						tem[k++] = bracket;
						bracket = '(';
					}
					right = calculate(tem);
					error = false;
					if (!right) ZeroMemory(tem, 15);
				}
			}

		}
	}
	if (right) strcpy_s(result,15, tem);
	return right;
}

//产生4个随机数字
void _24::creat() {
	ZeroMemory(result, 15);
	ZeroMemory(anwter, 15);
	for (int i = 0; i < 4; i++) {
		number[i] = rand() % 9 + 1;
	}
	while (!is_right())
	{
		for (int i = 0; i < 4; i++) {
			number[i] = rand() % 9 + 1;
		}
	}
}

queue<char> _24::change(char *expression) {
	queue<char> exp;
	stack<char> sign;
	char *p = expression;
	for (; *p != 0; p++) {
		if (*p <= '9'&&*p > '0') exp.push(*p);
		else {
			if (sign.empty() || *p == '(') {
				sign.push(*p);
			}
			else if ( *p == '+' || *p == '-') {
				while ( !sign.empty()&&sign.top() != '(')
				{
					exp.push(sign.top());
					sign.pop();
				}
				sign.push(*p);
			}
			else if (*p == '*' || *p == '/') {
				while (!sign.empty() && sign.top()!='('&&(sign.top() == '*'||sign.top() == '/'))
				{
					exp.push(sign.top());
					sign.pop();
				}
				sign.push(*p);
			}
			else if (*p == ')') {
				while (!sign.empty() && sign.top() != '(')
				{
					exp.push(sign.top());
					sign.pop();
				}
				if (!sign.empty() && sign.top() == '(') sign.pop();
				else error = true;
			}
			else error = true;
		}
	}
	while (!sign.empty() && sign.top() != '(')
	{
		exp.push(sign.top());
		sign.pop();
	}
	if (!sign.empty() && sign.top() == '(') error = true;
	return exp;
}

bool _24::calculate(char *expression) {
	queue<char> exp = change(expression);
	stack<int> tem;
	if (error) return false;
	map<int, int> num;
	int i, count=0,left,right;
	char sign;
	for (i = 0; i < 4; i++) {
		if (num.find(number[i]) == num.end()) num[number[i]] = 1;
		else num[number[i]]++;
	}
	while (!exp.empty())
	{
		if (exp.front() <= '9'&&exp.front() > '0') {
			if (num.find(exp.front() - '0') == num.end()) {
				error = true;
				return false;
			}
			else {
				num[exp.front() - '0']--;
				if (num[exp.front() - '0'] == 0) num.erase(exp.front() - '0');
			}
			tem.push(exp.front() - '0');
			exp.pop();
		}
		else {
			if (!tem.empty()) {
				right = tem.top();
				tem.pop();
			}
			if (!tem.empty()) {
				left = tem.top();
				tem.pop();
			}
			else {
				error = true;
				return false;
			}
			sign = exp.front();
			exp.pop();
			switch (sign)
			{
			case '+':
				left += right;
				break;
			case '-':
				left -= right;
				break;
			case '*':
				left *= right;
				break;
			case '/':
				//前面输入的时候已经排除了输入0的可能性
				if (left%right != 0) {
					error = true;
					return false;
				}
				left /= right;
				break;
			default:
				break;
			}
			tem.push(left); //将计算结果放入栈中 
		}
	}
	if (!num.empty()) {
		error = true;
		return false;
	}
	left = tem.top();
	if (left == 24) return true;
	else return false;
}

void _24::play() {
	creat();
	int score=0;             //记录玩家回答对的题目数量（分数）
	int level=1;             //记录当前关卡数
	int i,conue=1;
	bool res_right=false;
	while (conue)
	{
		cout << "当前分数为:" << score << endl;
		cout << "\n--------------------关卡" << level << "---------------------:" << endl;
		cout << "\n题目为:";
		for (i = 0; i < 4; i++) {
			cout << " " << number[i] << " ";
		}
		while (!error&&!res_right)
		{
			cout << "\n      请输入你的答案:" << endl;
			cin >> anwter;
			cin.ignore(1024, '\n');
			res_right = calculate(anwter);
			if (error == true) {
				cout << "输入的表达式有误,请重新输入" << endl;
				error = false;
			}
			else if (res_right == false) {
				cout << "\n你的答案错了哦.看看是哪里错了吧\n     输入  1,重新回答\n           2,查看答案并进入下一关(分数不增加)\n           0,退出游戏" << endl;
				std::cin >> i;
				if (i == 2) {
					cout << "\n正确答案为:" << result << endl;
					cout << "(按任意键进入下一关)" << endl;
					_getch();
					cout << "正在出题,请稍等" << endl;
					break;
				}
				else if (i == 0) {
					conue = 0;
					break;
				}
			}
		}
		if (res_right) {
			score++;
			cout << "真棒,回答正确,你目前的分数为:" << score << endl;
			cout << "是否要进入下一关（输入0退出游戏,输入1继续游戏)? " << endl;
			std::cin >> conue;
			if(conue) cout << "正在出题,请稍等" << endl;
		}
		res_right = false;
		level++;
		creat();
		system("CLS");
	}
	cout << "24点游戏旅程结束了,欢迎下次再来玩v" << endl;
}

int main()
{
	srand((unsigned)time(NULL));
	_24 player;
	player.start();
    return 0;
}

