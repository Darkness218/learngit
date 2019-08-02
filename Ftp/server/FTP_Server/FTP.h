#pragma once


#include <WinSock2.h>
#include <memory.h>
#include <ws2tcpip.h> 
#include <string>
#include <io.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace::std;

#define FTP_CON_PORT 21
#define FTP_DATA_PORT 20

class Ftp {
private:
	
	//string dir;       //���Ĭ��·��
public:
	SOCKET con_sock;  //����socket
	SOCKET data_sock;  //���ݴ���socket
	string cur_path;   //��ŵ�ǰ·��
	string dir;       //���Ĭ��·��
	Ftp();
	~Ftp();
	bool conection();
	bool data_conection();
	void getCurrentFile(vector<string>& files);
	int sendFile(SOCKET data_sock, string filename);
};