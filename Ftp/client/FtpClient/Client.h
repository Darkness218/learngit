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
	SOCKET con_sock;  //����socket
	SOCKET data_sock;  //���ݴ���socket
	string get_path;   //Ĭ������·��

	//��Ҫ��������
	void  handle();
	void login(SOCKET server_sock);
	bool conection();
	bool data_conection();
	void list();
	void get();
	void push();
	void pwd();
	void cd();

	//������������
	int get_line(SOCKET sock, char *buf, int size);
public:
	Client();
	~Client();
	void run();
};