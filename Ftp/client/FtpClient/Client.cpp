

#include "Client.h"

//建立控制连接
bool Client::conection() {
	int right = 0;
	char message[64];
	sockaddr_in addr;
	int len;
	char buf[64];

	con_sock = socket(AF_INET,SOCK_STREAM , 0);

	memset(&addr, '\0', sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(FTP_CON_PORT);
	inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

	right = connect(con_sock, (sockaddr *)&addr, sizeof(sockaddr_in));
	if (right == -1) {
		perror("connection connect faid");
		return false;
	}
	return true;
}

//建立数据连接
bool Client::data_conection() {
	int right = 0;
	char message[64];
	sockaddr_in addr;
	int len;
	char buf[64];

	data_sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&addr, '\0', sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(FTP_DATA_PORT);
	inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

	right = connect(data_sock, (sockaddr *)&addr, sizeof(sockaddr_in));
	if (right == -1) {
		perror("connection connect faid");
		return false;
	}
	return true;
}

//客户端启动函数
void Client::run() {
	bool right = false;
	right = conection();
	if (!right) {
		perror("Con conection");
		return;
	}
	cout << "与服务器连接成功" << endl;
	login(con_sock);
	cout << "登录成功,正在建立数据连接" << endl;
	right = data_conection();
	if (!right) {
		perror("data conection");
	}
	cout << "数据连接建立成功" << endl;
	handle();
	
}

//客户端登录函数
void Client::login(SOCKET sock) {
	bool right = false;
	while (!right) {
		char user[16]="";
		char pawd[10]="";
		char message[64]="";
		//输入用户名
		while (strcmp(message, "") == 0) {
			recv(sock, message, sizeof(message) - 1, 0);
		}
		cout << message;
		cin >> user;
		send(sock, user, sizeof(user) - 1, 0);

		//输入密码
		recv(sock, message, sizeof(message) - 1, 0);
		cout << message;
		cin >> pawd;
		send(sock, pawd, sizeof(user) - 1, 0);

		//验证结果
		recv(sock, message, sizeof(message) - 1, 0);
		if (strcmp(message, "密码错误或者账号不存在，请重新尝试\n") == 0) {
			cout << message << endl;
			continue;
		}
		else right = true;
	}
}

//读取一行的内容
int  Client::get_line(SOCKET sock, char *buf, int size) {
	int len;
	int i;
	char c;
	for (i = 0; i < size; i++) {
		len = recv(sock, &c, 1, 0);
		if (len == -1) {
			perror("read:");
			return -1;
		}
		if (c == '\n') {
			buf[i] = '\0';
			break;
		}
		else buf[i] = c;
	}
	if (i == size) buf[i] = '\0';

	return i;
}




Client::Client() {
	// 初始化socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 0);
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		perror("Init socket dll ");
		exit(1);
	}

	get_path = "E:\\C\\VS2017\\FtpClient\\get";
}


void  Client::handle() {
	int right = 0;
	while (!right) {
		char buf[128]="";
		char message[128]="请选择您要使用的功能: 1.dir 2.get 3.put 4.pwd 5.cd 6.quit ";
		/*while (strcmp(message, "请选择您要使用的功能: 1.list 2.retr 3.quit") != 0) {
			send(con_sock, "go on!", 10, 0);
			recv(con_sock, message, sizeof(message), 0);
		}*/
		cout << message << endl;
		cin>>buf;
		send(con_sock, buf, sizeof(buf), 0);
		recv(con_sock, message, sizeof(message), 0);
		if (strcmp(buf, "1")==0 || strcmp(buf, "dir") == 0) {
			list();
		}
		else if (strcmp(buf, "2") == 0 || strcmp(buf, "get") == 0) {
			get();
		}
		else if (strcmp(buf, "3") == 0 || strcmp(buf, "put") == 0) {
			push();
		}
		else if (strcmp(buf, "4") == 0 || strcmp(buf, "pwd") == 0) {
			pwd();
		}
		else if (strcmp(buf, "5") == 0 || strcmp(buf, "cd") == 0) {
			cd();
		}
		else if (strcmp(buf, "6") == 0 || strcmp(buf, "quit") == 0) {
			cout << "退出服务器" << endl;
			right = 1;
		}
		else cout << "你的选择有误，请重新确认您的选择" << endl;
		Sleep(100);
	}
}

Client::~Client() {
	closesocket(con_sock);
	closesocket(data_sock);
}

//得到服务器端文件列表
void Client::list() {
	int len = 1,i=0;
	while (len > 0) {
		char file[128];
		len = get_line(con_sock, file, sizeof(file)-1);
		if(strcmp(file,"over!")!=0) cout << '('<<++i<<')'<<file<<endl;
		else break;
	}
}

//从服务器下载文件
void Client::get() {
	char filename[64];
	bool right=false;
	ofstream file;
	cout << "请输入要下载的文件名(输入over!退出):" << endl;
	cin >> filename;
	
	//if()
	send(con_sock, filename, sizeof(filename), 0);
	if (strcmp(filename, "over!") == 0) {
		return;
	}
	file.open((get_path + "\\" + filename).c_str(), 'w');
	cout << "等待文件传输中" << endl;
	while (true) {
		char message[128] = "";
		recv(data_sock, message, sizeof(message),0);
		if (strcmp(message, "您输入的文件名有误请重新输入") == 0) break;
		else if(strcmp(message, "over!") != 0) file.write(message, strlen(message));
		else {
			right = true;
			break;
		}
	}
	file.close();
	if(right) cout << "\n文件传输完成" << endl;
	else {
		DeleteFileA((get_path + "\\" + filename).c_str());
		cout << "服务器没有该文件,请重新输入要下载的文件名" << endl;
		get();
	}
}

//上传文件至服务器
void Client::push() {
	char filename[64];
	ifstream file;
	bool right = false;
	while (!right) {
		cout << "请输入你要上传的文件名" << endl;
		cin >> filename;
		file.open(get_path+"\\"+filename);
		if (file.is_open()) right = true;
		else cout << "当前路径没有该文件,请重新输入文件名" << endl;
	}
	send(data_sock, filename, sizeof(filename), 0);
	cout << "已传输的文件内容为:" << endl;
	while (!file.eof()) {
		char buf[128] = "";
		int len;
		file.getline(buf, sizeof(buf));
		cout  << ":" << buf << endl;
		len = strlen(buf);
		if (len < sizeof(buf)) {
			buf[len++] = '\r';
			buf[len] = '\n';
		}
		send(data_sock, buf, sizeof(buf), 0);
	}
	send(data_sock, "over!", 10, 0);
	file.close();
}

void Client::pwd() {
	char path[64] = "";
	send(con_sock, "over!", 10, 0);
	while (strcmp(path, "") == 0) {
		recv(con_sock, path, sizeof(path) - 1,0);
	}
	cout << "当前路径为:" << path << endl;
}

void Client::cd() {
	char path[64];
	char message[128]="";
	cout << "请输入你要进入的路径名(输入0为默认路径)" << endl;
	cin.ignore(1012,'\n');
	cin.getline(path,64);
	send(con_sock, path, strlen(path), 0);
	while (strcmp(message, "") == 0||strcmp(message,"over!")==0) {
		recv(con_sock, message, sizeof(message), 0);
	}
	if (strcmp(message, "成功进入到该路径\n") == 0) {
		if (strcmp(path, "0") == 0) strcpy_s(path, "E:\\C\\VS2017\\FTP_Server\\directory");
		cout << "服务器成功进入路径" << path << endl;
	}
	else cout << "服务器没有该路径" << endl;
}