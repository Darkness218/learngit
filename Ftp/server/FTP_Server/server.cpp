

#include "Server.h"

//服务器启动函数
void Server::run() {
	bool right=false;

	cout << "Ftp服务器启动" << endl;
	right=ftp.conection();
	if (right) {
		cout << "等待客户端连接" << endl;
	}
	else {
		cout << "客户端链接失败" << endl;
		exit(1);
	}
	right=ftp.data_conection();
	if (right) {
		cout << "数据连接等待客户使用中" << endl;
	}
	else {
		cout << "客户端链接失败" << endl;
		exit(1);
	}
	handle();
}

/*
     服务器逻辑的实现
*/

//运行处理函数
void Server::handle() {
	int done = 1;
	
	while (done) {
		SOCKET server_sock,data_sock;
		sockaddr_in saddr,daddr;
	    char *user=new char[16];
		char username[16];
		int slen=0,dlen=0;
		char ip[64]="";
		thread td;

		memset(&saddr, '\0', sizeof(sockaddr_in));

		slen = sizeof(sockaddr_in);
		server_sock = accept(ftp.con_sock, (sockaddr *)&saddr, &slen);

		inet_ntop(AF_INET, &saddr.sin_addr.S_un.S_addr, ip, sizeof(ip) - 1);                     //建立控制连接
		if (strcmp(ip, "0.0.0.0") == 0) {
			closesocket(server_sock);
			continue;
		}
		cout << "\n IP端口为" << ip<<":"<< ntohs(saddr.sin_port)<< " 的客户连接成功 " << endl;
		user=login(server_sock);            //客户登录
		
		memset(&daddr, '\0', sizeof(sockaddr_in));
        dlen = sizeof(sockaddr_in);

		data_sock = accept(ftp.data_sock, (sockaddr *)&daddr, &dlen);              //建立数据连接
		strcpy_s(username, user);
		cout << "\n IP为" << ip << " 的客户"<<username<<"数据传输建立完成 " << endl;

		td = thread(&Server::server, server_sock, data_sock,ftp,username);
		td.detach();

	}
	system("PAUSE");
}

//处理客户登录函数,返回用户用户名
char * Server::login(SOCKET server_sock) {
	int right=0;
	bool success;
	while (!right) {
		char user[16] = "";
		char pawd[10] = "";
		char buf[64]="";
		right = send(server_sock, "user(请输入您的用户名:)\n", 25, 0);
		if (right == 0) {
			perror("connection Interrupt");
		}
		else if (right == -1) {
			perror("user send error");
		}
		right = recv(server_sock, user, sizeof(user) - 1, 0);
		if (right == 0) {
			perror("connection Interrupt");
		}
		else if (right == -1) {
			perror("user recv error");
		}

		right = send(server_sock, "pass(请输入您的密码:)\n", 23, 0);
		if (right == 0) {
			perror("connection Interrupt");
		}
		else if (right == -1) {
			perror("user send error");
		}
		right = recv(server_sock, pawd, sizeof(pawd) - 1, 0);
		if (right == 0) {
			perror("connection Interrupt");
		}
		else if (right == -1) {
			perror("user recv error");
		}
		success = database_read(user, pawd);
		if (!success) {
			 send(server_sock, "密码错误或者账号不存在，请重新尝试\n", 36, 0);
			 right = false;
		}
		else {
			send(server_sock, "登录成功!\n", 10, 0);
			cout << "\n用户名为" << user << "的客户登录成功" << endl;
			recv(server_sock, buf, sizeof(buf), 0);
			return user;
		}
		recv(server_sock, buf, sizeof(buf), 0);
	}
}



//处理服务器提供服务逻辑的函数
void Server::server(SOCKET server_sock, SOCKET data_sock, Ftp ftp, char *username) {
	bool right=false;
	while (!right) {
		char message[64]="";
		char buf[64]="";
		/*while (strcmp(message,"go on!")==0) {
			send(server_sock, "请选择您要使用的功能: 1.list 2.retr 3.quit", 100, 0);
			recv(server_sock, message, sizeof(message), 0);
		}*/
		while (strcmp(message, "") == 0) {
			recv(server_sock, message, sizeof(message), 0);
		}
		if (strcmp(message, "1")==0 || strcmp(message, "dir") == 0) {
			cout << "客户端"<<username<<":请求使用 dir 功能" << endl;
			send(server_sock, "", 1, 0);
			list(server_sock,ftp);
		}
		else if (strcmp(message, "2") == 0 || strcmp(message, "get") == 0) {
			cout << "客户端" << username << ":请求使用 get 功能" << endl;
			send(server_sock, "", 1, 0);
			retr(server_sock, data_sock,ftp);
		}
		else if (strcmp(message, "3") == 0 || strcmp(message, "put") == 0) {
			cout << "客户端" << username << ":请求使用put 功能" << endl;
			send(server_sock, "", 1, 0);
			stor(data_sock,ftp);
		}
		else if (strcmp(message, "4") == 0 || strcmp(message, "pwd") == 0) {
			cout << "客户端" << username << ":请求使用 pwd 功能" << endl;
			send(server_sock, "", 1, 0);
			pwd(server_sock, ftp);
		}
		else if (strcmp(message, "5") == 0 || strcmp(message, "cd") == 0) {
			cout << "客户端" << username << ":请求使用 cd 功能" << endl;
			cd(server_sock, ftp);
		}
		else if (strcmp(message, "6") == 0 || strcmp(message, "quit") == 0) {
			cout << "客户端" << username << ":请求使用 quit 功能" << endl;
			right = true;
		}
		else {
			send(server_sock, "您的选择有误,请重新选择", 24, 0);
		}  
	}
	closesocket(server_sock);
	closesocket(data_sock);
	cout << "客户端" << username << ":建立的链接释放成功" << endl;
}


/*
      服务内容的实现
*/

//客户端显示服务器当前目录文件的函数
void Server::list(SOCKET server_sock, Ftp ftp) {
	vector<string> files;
	ftp.getCurrentFile(files);

	cout << "当前路径下有以下文件:" << endl;
	vector<string>::iterator it;

	for (it = files.begin(); it< files.end(); it++) {
		string file = (*it).append("\n");
		cout << file.c_str() ;
		send(server_sock, file.c_str(), file.length(), 0);
	}
	send(server_sock, "over!\n", 7, 0);
}

//客户端文件下载函数
void Server::retr(SOCKET server_sock, SOCKET data_sock, Ftp ftp) {
	char file[64]="";
	int result = 0;
	while (strcmp(file, "") == 0) {
		recv(server_sock, file, sizeof(file), 0);
	}
	if (strcmp(file, "over!") == 0) {
		cout << "客户端取消get功能" << endl;
		return;
	}
	cout << "\n客户端请求下载名为" << file << " 的文件" << endl;
	result=ftp.sendFile(data_sock, file);
	if (result == -1) {
		retr(server_sock,data_sock,ftp);    //出错递归调用
	}
}

//客户端文件上传函数
void Server::stor( SOCKET data_sock, Ftp ftp) {
	char filename[64];
	ofstream file;
	recv(data_sock, filename, sizeof(filename), 0);
	cout << "正在接收文件" << filename << endl;
	file.open(ftp.cur_path + "\\" + filename);
	while (true) {
		char message[128] = "";
		recv(data_sock, message, sizeof(message), 0);
		if (strcmp(message, "over!") != 0) file.write(message, strlen(message));
		else break;
	}
	file.close();
	cout << "\n文件接收完成" << endl;
}

void Server::pwd(SOCKET server_sock, Ftp ftp) {
	string path = (ftp.cur_path ).c_str();
	char message[64]="";
	cout << "客户当前访问的路径为:" << path << endl;
	while (strcmp(message, "over!") != 0) {
		recv(server_sock, message, sizeof(message), 0);
	}
	send(server_sock, path.c_str(), path.length(), 0);
}


void Server::cd(SOCKET server_sock, Ftp &ftp) {
	char path[64]="";
	send(server_sock, "over!", 6, 0);
	while (strcmp(path, "") == 0) {
		recv(server_sock, path, sizeof(path), 0);
	}
	if (strcmp(path, "0") == 0 || strcmp(path, "\0") == 0) strcpy_s(path, ftp.dir.c_str()); 
	else if (PathFileExists(path) == false){
		send(server_sock, "非有效路径\n", 12,0);
		return;
	}
	ftp.cur_path = path;
	cout << "客户进入路径" << path<<endl;
	send(server_sock, "成功进入到该路径\n", 18, 0);
}



/*
       辅助函数的实现
*/

//从数据库中找出对应用户名，验证登录信息
bool Server::database_read(char *users, char* pawd) {
	MYSQL mysql, *sock; //声明MySQL的句柄 
	const char * host = "127.0.0.1"; //因为是作为本机测试，所以填写的是本地IP 
	const char * user = "root"; //这里改为你的用户名，即连接MySQL的用户名 
	const char * passwd = ""; //这里改为你的用户密码 
	const char * db = "study"; //这里改为你要连接的数据库的名字,一个数据可能有几张表
	unsigned int port = 3306; //这是MySQL的服务器的端口，如果你没有修改过的话就是3306。 
	const char * unix_socket = NULL; //unix_socket这是unix下的，我在Windows下，所以就把它设置为NULL 
	unsigned long client_flag = 0; //这个参数一般为0 

	const char * query = "select * from clientuser"; //查询语句，从那个表中查询,这里后面没有;

	MYSQL_RES * result = new MYSQL_RES; //保存结果集的
	MYSQL_ROW row; //代表的是结果集中的一行

	bool right = false;

	mysql_init(&mysql); //连接之前必须使用这个函数来初始化 
	if ((sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag)) == NULL) //连接MySQL 
	{
		perror("Mysql connection");
	}


	if (mysql_query(&mysql, query) != 0) //如果连接成功，则开始查询 .成功返回0
	{
		perror("Mysql query");
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //保存查询的结果 
		{
			perror("Mysql result");

		}
		else
		{
			while ((row = mysql_fetch_row(result)) != NULL) //读取结果集中的数据，返回的是下一行。因为保存结果集时，当前的游标在第一行【之前】 
			{
				if (strcmp(row[0], users) == 0) {
					if (strcmp(row[1], pawd) == 0) right = true;
					break;
				}
			}
		}

	}
	mysql_free_result(result); //释放结果集 
	mysql_close(sock); //关闭连接 

	return right;
}


