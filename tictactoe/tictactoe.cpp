// tictactoe.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <graphics.h>
#include<windows.h>
#include <time.h>
#include <iostream>
#include <vector>
using namespace::std;

#define win 1    //玩家赢了
#define lose -1  
#define draw 0  //平局
#define role 1
#define ai -1

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
	int computer_move;  //记录估值函数调用次数
public:
	player();
	void start();
	void play();
	bool is_win(vector<cheek> board, int sta);
	bool is_draw();
	void human_move();
	void ai_move();
	int evaluate(vector<cheek> board, int sta,int alpha, int beta,int detph);
};

player::player():chess_r(100),turn(0),ai_m(0),human_m(0),computer_move(0){
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
		if (sum == 3 * sta) return true;
		sum = board[i].cheekboard + board[i + 3].cheekboard + board[i + 6].cheekboard;
		if (sum == 3 * sta) return true;
	}
	sum = board[0].cheekboard + board[4].cheekboard + board[8].cheekboard;
	if (sum == 3 * sta) return true;
	sum = board[2].cheekboard + board[4].cheekboard + board[6].cheekboard;
	if (sum == 3 * sta) return true;
	return false;
}

//判断是否平局
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
	 evaluate(chessboard,ai,lose-1, win+ 1,0);
	chessboard[ai_m].cheekboard = -1;
	Sleep(turn*30);
	Beep(300, 500);   //ai行动之后蜂鸣提示
	computer_move = 0;
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

//计算ai_m的值确定ai的走步（ai估值用于返回给role极小值(-1)，role估值用于返回给ai极大值（1））,alpha向极大值靠拢，beta向极小值考虑
int player::evaluate(vector<cheek> board, int sta, int alpha, int beta,int depth) {
	//函数递归基准:上一次执方获胜,若ai获胜返回给role的估值函数1（win,玩家获胜），若为role获胜，返回给ai的估值函数-1（lose,玩家输了）
	if (is_win(board, -sta)) return sta;
	if (alpha == win) return win;
	if (beta == lose) return lose;
	computer_move++;
	int i, value, max;    //value为估值函数的结果，max为中间变量结果为value的极大值/极小值
	vector<cheek> new_board;    //声明一个临时的棋盘向量，在模拟走步的过程中不影响原先棋盘的状态（因为需要对原棋盘中各个空位置进行遍历，如果用旧棋盘会出现连续n次在同一个棋盘在走步二不是轮流走步）
	if(sta==ai)
	 max = lose-1;      //若为ai则是role调用，寻找极大值，max取极小-2（返回值最低为-1）
	else max = win + 1;  //若为role则是ai调用，寻找极小值，max取极大2（返回值最高为1）
	
	for (i = 0; i < 9; i++) {
		if (board[i].cheekboard == 0) {
			new_board = board;
			new_board[i].cheekboard = sta;   
			value=evaluate(new_board, -sta, alpha, beta, depth+1);     //ai调用，根据ai是否获胜返回
			switch (sta)
			{
			case 1:
				if (value < max)
				{
					max = value;
				}
				if (value < beta) beta = value;   //人走棋，选极小值，value值赋给bet
				if (alpha >= beta) return beta;     //若alpha值大，则返回beta，剪枝掉他的后继结点
				break;
			case -1:
				//若判断出输赢则令max更新
				if (value > max) {
					max = value;
					if (depth == 0) ai_m = i;  //si_m仅在第一层模拟走步时更新
				}
				if (value > alpha) alpha = value;  //ai走棋取极大值
				if (alpha >= beta) return alpha;   //ai获胜玩家失败时或者两方平局时成立，后续无需模拟，返回此时的极大值
				break;
			default:
				break;
			}
		}
	}
	if (sta==ai&&max == lose - 1||sta==role&&max==win+1) {
		return draw;   //若前面的过程分不出输赢(返回-1或1)则为平局，返回0
	}
	return max;    //若分出输赢返回1/-1
}



int main()
{
	srand((unsigned)time(NULL));
	player players;
	players.start();
    return 0;
}

