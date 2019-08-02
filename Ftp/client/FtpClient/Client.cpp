

#include "Client.h"

//������������
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

//������������
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

//�ͻ�����������
void Client::run() {
	bool right = false;
	right = conection();
	if (!right) {
		perror("Con conection");
		return;
	}
	cout << "����������ӳɹ�" << endl;
	login(con_sock);
	cout << "��¼�ɹ�,���ڽ�����������" << endl;
	right = data_conection();
	if (!right) {
		perror("data conection");
	}
	cout << "�������ӽ����ɹ�" << endl;
	handle();
	
}

//�ͻ��˵�¼����
void Client::login(SOCKET sock) {
	bool right = false;
	while (!right) {
		char user[16]="";
		char pawd[10]="";
		char message[64]="";
		//�����û���
		while (strcmp(message, "") == 0) {
			recv(sock, message, sizeof(message) - 1, 0);
		}
		cout << message;
		cin >> user;
		send(sock, user, sizeof(user) - 1, 0);

		//��������
		recv(sock, message, sizeof(message) - 1, 0);
		cout << message;
		cin >> pawd;
		send(sock, pawd, sizeof(user) - 1, 0);

		//��֤���
		recv(sock, message, sizeof(message) - 1, 0);
		if (strcmp(message, "�����������˺Ų����ڣ������³���\n") == 0) {
			cout << message << endl;
			continue;
		}
		else right = true;
	}
}

//��ȡһ�е�����
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
	// ��ʼ��socket dll
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
		char message[128]="��ѡ����Ҫʹ�õĹ���: 1.dir 2.get 3.put 4.pwd 5.cd 6.quit ";
		/*while (strcmp(message, "��ѡ����Ҫʹ�õĹ���: 1.list 2.retr 3.quit") != 0) {
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
			cout << "�˳�������" << endl;
			right = 1;
		}
		else cout << "���ѡ������������ȷ������ѡ��" << endl;
		Sleep(100);
	}
}

Client::~Client() {
	closesocket(con_sock);
	closesocket(data_sock);
}

//�õ����������ļ��б�
void Client::list() {
	int len = 1,i=0;
	while (len > 0) {
		char file[128];
		len = get_line(con_sock, file, sizeof(file)-1);
		if(strcmp(file,"over!")!=0) cout << '('<<++i<<')'<<file<<endl;
		else break;
	}
}

//�ӷ����������ļ�
void Client::get() {
	char filename[64];
	bool right=false;
	ofstream file;
	cout << "������Ҫ���ص��ļ���(����over!�˳�):" << endl;
	cin >> filename;
	
	//if()
	send(con_sock, filename, sizeof(filename), 0);
	if (strcmp(filename, "over!") == 0) {
		return;
	}
	file.open((get_path + "\\" + filename).c_str(), 'w');
	cout << "�ȴ��ļ�������" << endl;
	while (true) {
		char message[128] = "";
		recv(data_sock, message, sizeof(message),0);
		if (strcmp(message, "��������ļ�����������������") == 0) break;
		else if(strcmp(message, "over!") != 0) file.write(message, strlen(message));
		else {
			right = true;
			break;
		}
	}
	file.close();
	if(right) cout << "\n�ļ��������" << endl;
	else {
		DeleteFileA((get_path + "\\" + filename).c_str());
		cout << "������û�и��ļ�,����������Ҫ���ص��ļ���" << endl;
		get();
	}
}

//�ϴ��ļ���������
void Client::push() {
	char filename[64];
	ifstream file;
	bool right = false;
	while (!right) {
		cout << "��������Ҫ�ϴ����ļ���" << endl;
		cin >> filename;
		file.open(get_path+"\\"+filename);
		if (file.is_open()) right = true;
		else cout << "��ǰ·��û�и��ļ�,�����������ļ���" << endl;
	}
	send(data_sock, filename, sizeof(filename), 0);
	cout << "�Ѵ�����ļ�����Ϊ:" << endl;
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
	cout << "��ǰ·��Ϊ:" << path << endl;
}

void Client::cd() {
	char path[64];
	char message[128]="";
	cout << "��������Ҫ�����·����(����0ΪĬ��·��)" << endl;
	cin.ignore(1012,'\n');
	cin.getline(path,64);
	send(con_sock, path, strlen(path), 0);
	while (strcmp(message, "") == 0||strcmp(message,"over!")==0) {
		recv(con_sock, message, sizeof(message), 0);
	}
	if (strcmp(message, "�ɹ����뵽��·��\n") == 0) {
		if (strcmp(path, "0") == 0) strcpy_s(path, "E:\\C\\VS2017\\FTP_Server\\directory");
		cout << "�������ɹ�����·��" << path << endl;
	}
	else cout << "������û�и�·��" << endl;
}