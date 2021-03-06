// 2048.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <graphics.h>
#include <time.h>
#include <tchar.h>
#include <conio.h>
#include <vector>
#include <stack>
#include <ostream>
using namespace::std;

class _2048 {
private:
	//结构体，每一个方格包含左上角坐标以及方格内的数字，0为空
	struct grit {
		int x;
		int y;
		int status;
		grit(int a, int b, int sta=0) :x(a),y(b),status(sta){

		}
	};
	vector<grit>  checker;      //数组储存棋盘
	vector<grit>  last_move;   //记录上一次方格状态，撤回用
	TCHAR number[10];
	int grit_r;  //方格半径
	int score;
	int last_score;    //记录上一次行动的分数，撤回用
	bool ref;  //是否需要更新数字2
	void play();
	bool is_end();
	void move(char ope);
	void remove();
	void refresh();
public:
	_2048();
	void start();
};

//构造器，设置方格位置
_2048::_2048():grit_r(100),score(0),ref(1),last_score(0) {
	int i;
	for (i = 0; i < 16; i++) {
		checker.push_back(grit((i%4+1)*100, (i/4+1)*100, 0));
	}
}


//游戏开始界面
void _2048::start() {
	int key = 1;
	MOUSEMSG mos; //鼠标对象
	initgraph(720, 640);
	settextcolor(RED);
	settextstyle(64, 48, _T("楷体"));
	outtextxy(720 / 2 - 64 * 2, 40, _T("2048"));
	settextcolor(BLUE);
	settextstyle(48, 32, _T("楷体"));
	outtextxy(720 / 2 - 48 * 4, 48 + 32 * 6, _T("开始游戏"));
	outtextxy(720 / 2 - 48 * 4, 48 + 32 * 9, _T("游戏规则"));
	outtextxy(720 / 2 - 48 * 4, 48 + 32 * 12, _T("结束游戏"));
	while (key)
	{
		mos = GetMouseMsg();
		switch (mos.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((mos.x >= 720 / 2 - 48 * 4 && mos.x <= 720 / 2 + 48) && (mos.y >= 48 + 32 * 6 && mos.y <= 48 + 32 * 6 + 32 * 1.5)) {
				cleardevice();
				play();
				key = 0;
			}
			else if ((mos.x >= 720 / 2 - 48 * 4 && mos.x <= 720 / 2 + 48) && (mos.y >= 48 + 32 * 9 && mos.y <= 48 + 32 * 9 + 32 * 1.5)) {
				MessageBox(NULL, _T("通过W,S,A,D控制方块移动，相同的方块会合并,当没有方块可以合并时游戏结束,红色的方块为新生成的方块(J键可以撤回一次操作)"), _T("游戏规则"), MB_OK);
			}
			else if ((mos.x >= 720 / 2 - 48 * 4 && mos.x <= 720 / 2 + 48) && (mos.y >= 48 + 32 * 12 && mos.y <= 48 + 32 * 12 + 32 * 1.5))
			{
				closegraph();
				_exit(0);
				return;
			}
			break;
		case WM_RBUTTONUP:
			_exit(0);
			break;
		}
	}
	closegraph();
	start();
}

//判断游戏是否结束
bool _2048::is_end(){
	int i;
	for (i = 0; i < 16; i++) {
		if (checker[i].status == 0) return false;
		if(i-1<4 +  i / 4 *4&&i-1>=0 + i / 4*4 &&checker[i-1].status== checker[i].status) return false;
		if (i + 1<4+i/4*4 && i + 1 >= 0 +  i / 4*4 && checker[i + 1].status == checker[i].status) return false;
		if (i - 4<16 && i - 4 >= 0 && checker[i - 4].status == checker[i].status) return false;
		if (i + 4<16 && i + 4 >= 0 && checker[i + 4].status == checker[i].status) return false;
	}
	return true;
}

//游戏运行函数
void _2048::play() {
	refresh();
	char moves;
	int i;
	while (!is_end()) {
		moves = _getch();
		if(moves!='j') last_move=checker;
		if (moves != 'w'&& moves != 's' &&moves != 'a' && moves != 'd'&&moves!='j') continue;
		move(moves);
		if (!last_move.empty()) {
			for (i = 0; i < 16; i++) {
				if (last_move[i].status != checker[i].status) break;
			}
		}
		if (moves == 'j' || i != 16) refresh();
	}
	for (int i = 0; i < 16; i++) checker[i].status = 0;
	MessageBox(NULL, _T("很抱歉，游戏结束"), _T("游戏结束"), MB_OK);
}

//撤回操作
void _2048::remove() {
	ref = 0;
	if (last_move.empty()) return;
	checker = last_move;
	score = last_score;
	last_score = 0;
	last_move.clear();
}


//移动操作
void _2048::move(char ope) {
	if(ope!='j') last_score = score;
	int i,j,t=0;
	stack<int> tem;
	stack<int> merge;
	int count[4] = { 0 };
	switch (ope)
	{
	case 'w':
		for (i = 0; i <4; i++) {
			for (j = 0; j <4; j++) {
				if (checker[15-i-j*4].status != 0) {
					tem.push(checker[15 - i - j * 4].status);
					checker[15 - i - j * 4].status = 0;
					count[(3-i) % 4]++;
				}
			}
		}
		for (i = 0; i <4;) {
			for (j = 0; j < 4; j++) {
				if (count[i] == 0) {
					i++;
					break;
				}
				checker[i+j*4].status = tem.top();
				count[i]--;
				tem.pop();
				if (count[i] != 0 && checker[i+j*4].status == tem.top()) {
					checker[i+j*4].status  *= 2;
					count[i]--;
					tem.pop();
					score += checker[i + j * 4].status;
				}
			}
		}
		break;
	case 's':
		for (i = 0; i <4; i++) {
			for (j = 3; j >=0; j--) {
				if (checker[15 - i - j * 4].status != 0) {
					tem.push(checker[15 - i - j * 4].status);
					checker[15 - i - j * 4].status = 0;
					count[(3 - i) % 4]++;
				}
			}
		}
		for (i = 0; i <4;) {
			if (count[i] == 0) {
				i++;
				continue;
			}
			//合并
			for (j = 0; j < count[i] ; j++) {
				if (!merge.empty()&&merge.top() == tem.top()) {
					merge.pop();
					merge.push(tem.top()*2);
					count[i]--;
					j--;
					score += merge.top();
				}
				else merge.push(tem.top());
				tem.pop();
			}
			for (j = 4 - count[i]; j <4; j++) {
				checker[i + j * 4].status = merge.top();
				merge.pop();
			}
			count[i] = 0;
		}
		break;
	case 'a':
		
		for (i = 15; i >= 0; i--) {
			if (checker[i].status != 0) {
				tem.push(checker[i].status);
				checker[i].status = 0;
				count[i / 4]++;  
			}
		}
		for (i = 0; i < 4;) {
			if (count[i] == 0) {
				i++;
				t =0;
				continue;
			}
			if (t == 0) t = count[i];
			//移动操作
			checker[ t-count[i] + i * 4].status = tem.top();
			count[i]--;
			tem.pop();
			//合并操作
			if (count[i] != 0 && checker[t - count[i]-1 + i * 4 ].status == tem.top()) {
				checker[t - count[i] - 1 + i * 4 ].status *= 2;
				t--;
				count[i]--;
				tem.pop();
				score += checker[t - count[i] - 1 + i * 4].status;
			}
		}
		break;
	case 'd':
		
		for (i = 0; i <16; i++) {
			//把所有数字放入栈中
			if (checker[i].status != 0) {
				tem.push(checker[i].status);
				checker[i].status = 0;
				count[i / 4]++;  //记录每行的数字数量
			}
		}
		for (i = 3; i >=0;) {
			if (count[i] == 0) {
				i--;
				continue;
			}
			for (j = 0; j < count[i]; j++) {
				if (!merge.empty() && merge.top() == tem.top()) {
					merge.pop();
					merge.push(tem.top() * 2);
					count[i]--;
					j--;
					score += merge.top();
				}
				else merge.push(tem.top());
				tem.pop();
			}
			for (j = 4 - count[i]; j <4; j++) {
				checker[i*4 + j ].status = merge.top();
				merge.pop();
			}
			count[i] = 0;
		}
		break;
	case 'j':
		remove();
		break;
	default:
		break;
	}

}

//刷新
void _2048::refresh() {
	cleardevice();
	setlinecolor(WHITE);
	int i;
	int key;
	TCHAR s[10];
	//循环找到随机生成2的点
	for (i = 0; i < 16; i++) if (checker[i].status == 0) break;
	if(i<16) while (checker[key = rand() % 16].status != 0)
	{
		key = rand() % 16;
	}
	else ref = 0;
	if(ref) checker[key].status = 2;   //随机产生一个2
	else ref = 1;
	for (i = 0; i < 16; i++) {
		rectangle(checker[i].x, checker[i].y, checker[i].x+grit_r, checker[i].y + grit_r);
		_stprintf_s(number, _T("%d"), checker[i].status);
		//根据数字设置文本格式
		switch (checker[i].status)
		{
		case 2: 
		case 4: 
		case 8: 
			settextstyle(80, 80, _T("楷体"));
			break;
		case 16:
		case 32:
		case 64:
			settextstyle(35, 40, _T("楷体"));
			break;
		case 128:
		case 256:
		case 512:
			settextstyle(25, 40, _T("楷体"));
			break;
		case 1024:
		case 2048:
			settextstyle(20, 40, _T("楷体"));
			break;
		default:
			settextstyle(15, 40, _T("楷体"));
			break;
		}
		if(i!=key) settextcolor(YELLOW);
		else settextcolor(RED);  //新产生的2用红色标出
		if(checker[i].status!=0) outtextxy(checker[i].x + 10, checker[i].y + 10, number);
	}
	_stprintf_s(s, _T("%d"), score/2);
	settextcolor(BLUE);
	settextstyle(30, 30, _T("楷体"));
	outtextxy(520, 200, _T("score:"));
	outtextxy(550, 250, s);
}

int main()
{
	srand((unsigned)time(NULL));
	_2048 game;
	game.start();
	return 0;
}

