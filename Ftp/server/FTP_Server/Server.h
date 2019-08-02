#pragma once


#include "FTP.h"
#include   "shlwapi.h"
#include <mysql.h>
#include <thread>
using namespace::std;
#pragma comment(lib,"ws2_32.lib")
#pragma comment( lib, "shlwapi.lib")


class Server {
private:
	Ftp ftp;

	//主要工作函数
	void  handle();
	char * login(SOCKET server_sock);
	static void server(SOCKET server_sock, SOCKET data_sock,Ftp ftp,char *username);

	static void pwd(SOCKET server_sock, Ftp ftp);
	static void retr(SOCKET server_sock,SOCKET data_sock, Ftp ftp);
	static void list(SOCKET server_sock, Ftp ftp);
	static void stor(SOCKET data_sock, Ftp ftp);
	static void cd(SOCKET server_sock, Ftp &ftp);
	static void rename(SOCKET server_sock, Ftp ftp);
	//辅助工作函数
	bool database_read(char *users, char* pawd);
public:
	void run();
};