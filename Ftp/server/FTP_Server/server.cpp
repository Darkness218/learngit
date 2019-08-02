

#include "Server.h"

//��������������
void Server::run() {
	bool right=false;

	cout << "Ftp����������" << endl;
	right=ftp.conection();
	if (right) {
		cout << "�ȴ��ͻ�������" << endl;
	}
	else {
		cout << "�ͻ�������ʧ��" << endl;
		exit(1);
	}
	right=ftp.data_conection();
	if (right) {
		cout << "�������ӵȴ��ͻ�ʹ����" << endl;
	}
	else {
		cout << "�ͻ�������ʧ��" << endl;
		exit(1);
	}
	handle();
}

/*
     �������߼���ʵ��
*/

//���д�����
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

		inet_ntop(AF_INET, &saddr.sin_addr.S_un.S_addr, ip, sizeof(ip) - 1);                     //������������
		if (strcmp(ip, "0.0.0.0") == 0) {
			closesocket(server_sock);
			continue;
		}
		cout << "\n IP�˿�Ϊ" << ip<<":"<< ntohs(saddr.sin_port)<< " �Ŀͻ����ӳɹ� " << endl;
		user=login(server_sock);            //�ͻ���¼
		
		memset(&daddr, '\0', sizeof(sockaddr_in));
        dlen = sizeof(sockaddr_in);

		data_sock = accept(ftp.data_sock, (sockaddr *)&daddr, &dlen);              //������������
		strcpy_s(username, user);
		cout << "\n IPΪ" << ip << " �Ŀͻ�"<<username<<"���ݴ��佨����� " << endl;

		td = thread(&Server::server, server_sock, data_sock,ftp,username);
		td.detach();

	}
	system("PAUSE");
}

//����ͻ���¼����,�����û��û���
char * Server::login(SOCKET server_sock) {
	int right=0;
	bool success;
	while (!right) {
		char user[16] = "";
		char pawd[10] = "";
		char buf[64]="";
		right = send(server_sock, "user(�����������û���:)\n", 25, 0);
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

		right = send(server_sock, "pass(��������������:)\n", 23, 0);
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
			 send(server_sock, "�����������˺Ų����ڣ������³���\n", 36, 0);
			 right = false;
		}
		else {
			send(server_sock, "��¼�ɹ�!\n", 10, 0);
			cout << "\n�û���Ϊ" << user << "�Ŀͻ���¼�ɹ�" << endl;
			recv(server_sock, buf, sizeof(buf), 0);
			return user;
		}
		recv(server_sock, buf, sizeof(buf), 0);
	}
}



//����������ṩ�����߼��ĺ���
void Server::server(SOCKET server_sock, SOCKET data_sock, Ftp ftp, char *username) {
	bool right=false;
	while (!right) {
		char message[64]="";
		char buf[64]="";
		/*while (strcmp(message,"go on!")==0) {
			send(server_sock, "��ѡ����Ҫʹ�õĹ���: 1.list 2.retr 3.quit", 100, 0);
			recv(server_sock, message, sizeof(message), 0);
		}*/
		while (strcmp(message, "") == 0) {
			recv(server_sock, message, sizeof(message), 0);
		}
		if (strcmp(message, "1")==0 || strcmp(message, "dir") == 0) {
			cout << "�ͻ���"<<username<<":����ʹ�� dir ����" << endl;
			send(server_sock, "", 1, 0);
			list(server_sock,ftp);
		}
		else if (strcmp(message, "2") == 0 || strcmp(message, "get") == 0) {
			cout << "�ͻ���" << username << ":����ʹ�� get ����" << endl;
			send(server_sock, "", 1, 0);
			retr(server_sock, data_sock,ftp);
		}
		else if (strcmp(message, "3") == 0 || strcmp(message, "put") == 0) {
			cout << "�ͻ���" << username << ":����ʹ��put ����" << endl;
			send(server_sock, "", 1, 0);
			stor(data_sock,ftp);
		}
		else if (strcmp(message, "4") == 0 || strcmp(message, "pwd") == 0) {
			cout << "�ͻ���" << username << ":����ʹ�� pwd ����" << endl;
			send(server_sock, "", 1, 0);
			pwd(server_sock, ftp);
		}
		else if (strcmp(message, "5") == 0 || strcmp(message, "cd") == 0) {
			cout << "�ͻ���" << username << ":����ʹ�� cd ����" << endl;
			cd(server_sock, ftp);
		}
		else if (strcmp(message, "6") == 0 || strcmp(message, "quit") == 0) {
			cout << "�ͻ���" << username << ":����ʹ�� quit ����" << endl;
			right = true;
		}
		else {
			send(server_sock, "����ѡ������,������ѡ��", 24, 0);
		}  
	}
	closesocket(server_sock);
	closesocket(data_sock);
	cout << "�ͻ���" << username << ":�����������ͷųɹ�" << endl;
}


/*
      �������ݵ�ʵ��
*/

//�ͻ�����ʾ��������ǰĿ¼�ļ��ĺ���
void Server::list(SOCKET server_sock, Ftp ftp) {
	vector<string> files;
	ftp.getCurrentFile(files);

	cout << "��ǰ·�����������ļ�:" << endl;
	vector<string>::iterator it;

	for (it = files.begin(); it< files.end(); it++) {
		string file = (*it).append("\n");
		cout << file.c_str() ;
		send(server_sock, file.c_str(), file.length(), 0);
	}
	send(server_sock, "over!\n", 7, 0);
}

//�ͻ����ļ����غ���
void Server::retr(SOCKET server_sock, SOCKET data_sock, Ftp ftp) {
	char file[64]="";
	int result = 0;
	while (strcmp(file, "") == 0) {
		recv(server_sock, file, sizeof(file), 0);
	}
	if (strcmp(file, "over!") == 0) {
		cout << "�ͻ���ȡ��get����" << endl;
		return;
	}
	cout << "\n�ͻ�������������Ϊ" << file << " ���ļ�" << endl;
	result=ftp.sendFile(data_sock, file);
	if (result == -1) {
		retr(server_sock,data_sock,ftp);    //����ݹ����
	}
}

//�ͻ����ļ��ϴ�����
void Server::stor( SOCKET data_sock, Ftp ftp) {
	char filename[64];
	ofstream file;
	recv(data_sock, filename, sizeof(filename), 0);
	cout << "���ڽ����ļ�" << filename << endl;
	file.open(ftp.cur_path + "\\" + filename);
	while (true) {
		char message[128] = "";
		recv(data_sock, message, sizeof(message), 0);
		if (strcmp(message, "over!") != 0) file.write(message, strlen(message));
		else break;
	}
	file.close();
	cout << "\n�ļ��������" << endl;
}

void Server::pwd(SOCKET server_sock, Ftp ftp) {
	string path = (ftp.cur_path ).c_str();
	char message[64]="";
	cout << "�ͻ���ǰ���ʵ�·��Ϊ:" << path << endl;
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
		send(server_sock, "����Ч·��\n", 12,0);
		return;
	}
	ftp.cur_path = path;
	cout << "�ͻ�����·��" << path<<endl;
	send(server_sock, "�ɹ����뵽��·��\n", 18, 0);
}



/*
       ����������ʵ��
*/

//�����ݿ����ҳ���Ӧ�û�������֤��¼��Ϣ
bool Server::database_read(char *users, char* pawd) {
	MYSQL mysql, *sock; //����MySQL�ľ�� 
	const char * host = "127.0.0.1"; //��Ϊ����Ϊ�������ԣ�������д���Ǳ���IP 
	const char * user = "root"; //�����Ϊ����û�����������MySQL���û��� 
	const char * passwd = ""; //�����Ϊ����û����� 
	const char * db = "study"; //�����Ϊ��Ҫ���ӵ����ݿ������,һ�����ݿ����м��ű�
	unsigned int port = 3306; //����MySQL�ķ������Ķ˿ڣ������û���޸Ĺ��Ļ�����3306�� 
	const char * unix_socket = NULL; //unix_socket����unix�µģ�����Windows�£����ԾͰ�������ΪNULL 
	unsigned long client_flag = 0; //�������һ��Ϊ0 

	const char * query = "select * from clientuser"; //��ѯ��䣬���Ǹ����в�ѯ,�������û��;

	MYSQL_RES * result = new MYSQL_RES; //����������
	MYSQL_ROW row; //������ǽ�����е�һ��

	bool right = false;

	mysql_init(&mysql); //����֮ǰ����ʹ�������������ʼ�� 
	if ((sock = mysql_real_connect(&mysql, host, user, passwd, db, port, unix_socket, client_flag)) == NULL) //����MySQL 
	{
		perror("Mysql connection");
	}


	if (mysql_query(&mysql, query) != 0) //������ӳɹ�����ʼ��ѯ .�ɹ�����0
	{
		perror("Mysql query");
	}
	else
	{
		if ((result = mysql_store_result(&mysql)) == NULL) //�����ѯ�Ľ�� 
		{
			perror("Mysql result");

		}
		else
		{
			while ((row = mysql_fetch_row(result)) != NULL) //��ȡ������е����ݣ����ص�����һ�С���Ϊ��������ʱ����ǰ���α��ڵ�һ�С�֮ǰ�� 
			{
				if (strcmp(row[0], users) == 0) {
					if (strcmp(row[1], pawd) == 0) right = true;
					break;
				}
			}
		}

	}
	mysql_free_result(result); //�ͷŽ���� 
	mysql_close(sock); //�ر����� 

	return right;
}


