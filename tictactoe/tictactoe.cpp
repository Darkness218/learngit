// tictactoe.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <graphics.h>
#include <time.h>
#include <iostream>
#include <vector>
using namespace::std;

#define win 1    
#define lose -1
#define draw 0
#define role -1
#define ai 1

class player {
private:
	
	struct cheek
	{
		int cheekboard;   //格子状态
		int x, y;         //格子位置
		cheek(int a=0,int b=0,int sta=0):x(a),y(b),cheekboard(sta){}
	};
	int status;    //执方
	vector<cheek> chessboard;   //棋盘数组
	int human_m;      //记录人的行动
	int ai_m;         //记录ai的行动
	int chess_r;  //格子大小
	int turn;   //回合数
public:
	player();
	void start();
	void play();
	bool is_win(vector<cheek> board, int sta);
	bool is_draw();
	void human_move();
	void ai_move();
	int evaluate_ai(vector<cheek> board, int alpha, int beta,int detph);
	int evaluate_human(vector<cheek> board, int alpha, int beta);
};

player::player():chess_r(100),turn(0),ai_m(0),human_m(0){
	int i=rand()%2;
	int x, y;
	x = y = 100;
	if (!i) status = role;
	else status = ai;
	chessboard.resize(9);
	for (i = 0; i < 9; i++) {
		chessboard[i] = cheek(x,y,0);
		if (x < 300) x += chess_r;
		else x = 100;
		if (i&&(i+1) % 3 == 0) y += chess_r;
	}
}

//判断是否赢
bool player::is_win(vector<cheek> board,int sta) {
	int sum;  //记录某一直线的和(若和为-3ai赢，若为3则玩家赢)
	int i;
	for (i = 0; i < 3; i++) {
		sum = board[i * 3].cheekboard + board[i*3 + 1].cheekboard + board[i*3 + 2].cheekboard;
		if (sum == -3 * sta) return true;
		sum = board[i].cheekboard + board[i + 3].cheekboard + board[i + 6].cheekboard;
		if (sum == -3 * sta) return true;
	}
	sum = board[0].cheekboard + board[4].cheekboard + board[8].cheekboard;
	if (sum == -3 * sta) return true;
	sum = board[2].cheekboard + board[4].cheekboard + board[6].cheekboard;
	if (sum == -3 * sta) return true;
	return false;
}

bool player::is_draw() {
	if (turn != 9) return false;
	if (is_win(chessboard, -status)) return false;
	for (int i = 0; i < 9; i++) {
		if (chessboard[i].cheekboard == 0) return false;
	}
	return true;
}

//执方为人
void player::human_move() {
	MOUSEMSG m;
	m = GetMouseMsg();
	int i;
	for (i = 0; i < 9; i++) {
		if (m.uMsg== WM_LBUTTONDOWN&&m.x > chessboard[i].x&&m.x<chessboard[i].x + chess_r && m.y>chessboard[i].y&&m.y < chessboard[i].y + chess_r) {
			if (chessboard[i].cheekboard != 0) {
				human_move();
				break;
			}
			else {
				chessboard[i].cheekboard = 1;
				human_m = i;
			}
			break;
		}
	}

	if (i == 9) human_move();
	status = ai;
}

//执方为电脑
void player::ai_move() {
	status = role;
	 evaluate_ai(chessboard,lose-1, win+ 1,0);
	chessboard[ai_m].cheekboard = -1;
	Sleep(300);
}


//开始函数
void player::start() {
	bool right;
	int key = 1;
	MOUSEMSG mos; //鼠标对象
	initgraph(640, 480);
	settextcolor(RED);
	settextstyle(64, 48, _T("楷体"));
	outtextxy(640 / 2 - 64 * 2, 40, _T("井字棋"));
	settextcolor(BLUE);
	settextstyle(48, 32, _T("楷体"));
	outtextxy(640 / 2 - 48 * 4, 48 + 32 * 3, _T("开始游戏"));
	outtextxy(640 / 2 - 48 * 4, 48 + 32 * 6, _T("游戏规则"));
	outtextxy(640 / 2 - 48 * 4, 48 + 32 * 9, _T("结束游戏"));
	while (key)
	{
		mos = GetMouseMsg();
		switch (mos.uMsg)
		{
		case WM_LBUTTONDOWN:
			if ((mos.x >= 640 / 2 - 48 * 4 && mos.x <= 640 / 2 + 48) && (mos.y >= 48 + 32 * 3 && mos.y <= 48 + 32 * 3 + 32 * 1.5)) {
				cleardevice();
				play();
				key = 0;
			}
			else if ((mos.x >= 640 / 2 - 48 * 4 && mos.x <= 640 / 2 + 48) && (mos.y >= 48 + 32 * 6 && mos.y <= 48 + 32 * 6 + 32 * 1.5)) {
				MessageBox(NULL, _T("玩家与电脑轮流执棋，当一方棋子3个子连成一条直线时获胜"), _T("游戏规则"), MB_OK);
			}
			else if ((mos.x >= 640 / 2 - 48 * 4 && mos.x <= 640 / 2 + 48) && (mos.y >= 48 + 32 * 9 && mos.y <= 48 + 32 * 9 + 32 * 1.5))
			{
				closegraph();
				cout << "游戏结束" << endl;
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

void player::play() {
	int i;
	cheek current;  //记录当前格子位置
	setlinecolor(WHITE);
	TCHAR h[] = _T("玩家思考中");
	TCHAR a[] = _T("电脑思考中");
	settextcolor(BLUE);
	settextstyle(32,24,_T("楷体"));
	//绘制棋盘
	line(chessboard[1].x, chessboard[1].y, chessboard[7].x, chessboard[7].y + chess_r);
	line(chessboard[2].x, chessboard[2].y, chessboard[8].x, chessboard[8].y + chess_r);
	line(chessboard[3].x, chessboard[3].y, chessboard[5].x+chess_r, chessboard[5].y);
	line(chessboard[6].x, chessboard[6].y, chessboard[8].x + chess_r, chessboard[8].y);
	setlinecolor(RED);
	while (!is_win(chessboard, -status)&&!is_draw())
	{
		if (status == ai) {
			outtextxy(chessboard[2].x, chessboard[1].y-chess_r,a);
			ai_move();
			current = chessboard[ai_m];
		}
		else {
			outtextxy(chessboard[2].x , chessboard[1].y - chess_r, h);
			human_move();
			current = chessboard[human_m];
		}
		if (turn % 2 == 0) circle(current.x + chess_r / 2, current.y + chess_r / 2, chess_r / 2 - 10);
		else {
			line(current.x + 10, current.y+10, current.x + chess_r -10, current.y + chess_r-10 );
			line(current.x + chess_r - 10, current.y +10, current.x+ 10, current.y + chess_r - 10);
		}
		turn++;
	}
	Sleep(300);
	cleardevice();
	settextstyle(64,36,_T("楷体"));
	settextcolor(RED);
	if (is_draw()) {
		outtextxy(220, 200, _T("平局"));
	}
	else if (status == role) {
		outtextxy(220, 200, _T("你输了"));
	}
	else {
		outtextxy(220, 200, _T("你赢了"));
	}

	Sleep(1000);
	turn = 0;
	ai_m = human_m = 0;
	i = rand() % 2;
	if (!i) status = role;
	else status = ai;
	for (i = 0; i < 9; i++) {
		chessboard[i].cheekboard = 0;
	}
	
}

int player::evaluate_ai(vector<cheek> board, int alpha, int beta,int depth) {
	if (is_win(board, role)) return lose;
	int i, value, max;
	vector<cheek> new_board;
	 max = lose-1;
	
	for (i = 0; i < 9; i++) {
		if (board[i].cheekboard == 0) {
			new_board = board;
			new_board[i].cheekboard = -1;
			value=evaluate_human(new_board,alpha, beta);
			if (value > max) {
				max = value;
				if (depth == 0) ai_m = i;
			}
			if (value > alpha) alpha = value;  //取极大值
			if (alpha >= beta) return alpha;   //取极大值，剪枝掉后继结点
		}
	}
	if (max == lose - 1) {
		return draw;   //相等为“平局”
	}
	return max;
}

int player::evaluate_human(vector<cheek> board, int alpha, int beta) {
	if (is_win(board, ai)) return win;
	int i, value, min;
	vector<cheek> new_board;
	min=win+1;

	for (i = 0; i < 9; i++) {
		if (board[i].cheekboard == 0) {
			new_board = board;
			new_board[i].cheekboard = 1;
			value = evaluate_ai(new_board, alpha, beta,1);
			if (value < min)
			{
				min = value;
			}
			if (value < beta) beta = value;   //人走棋，选极小值，value值赋给beta
			if (alpha >= beta) return beta;     //若alpha值大，则返回beta，剪枝掉他的后继结点
		}
	}
	if (min==win+1) {
		return draw;   //相等为“平局”
	}
	return min;
}

int main()
{
	srand((unsigned)time(NULL));
	player players;
	players.start();
    return 0;
}

