#pragma once


#include <WinSock2.h>
#include <memory.h>
#include <ws2tcpip.h> 
#include <iostream>
#include <fstream>
using namespace::std;
#pragma comment(lib,"ws2_32.lib")

#define FTP_CON_PORT 21
#define FTP_DATA_PORT 20
#define SERVER_IP "127.0.0.1"

class Client {
private:
	SOCKET con_sock;  //连接socket
	SOCKET data_sock;  //数据传输socket
	string get_path;   //默认下载路径

	//主要工作函数
	void  handle();
	void login(SOCKET server_sock);
	bool conection();
	bool data_conection();
	void list();
	void get();
	void push();
	void pwd();
	void cd();

	//辅助工作函数
	int get_line(SOCKET sock, char *buf, int size);
public:
	Client();
	~Client();
	void run();
};