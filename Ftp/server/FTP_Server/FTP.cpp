
#include "FTP.h"

Ftp::Ftp() {
	// 初始化socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 0);
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		perror("Init socket dll ");
		exit(1);
	}
    
	dir .append("E:\\C\\VS2017\\FTP_Server\\directory");
	cur_path = dir;
}

Ftp::~Ftp() {
	closesocket(con_sock);
	closesocket(data_sock);
	
}

//与客户端进行控制连接
bool Ftp::conection() {
	sockaddr_in addr;
	int right;
	int addr_len;


	con_sock = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	
	memset(&addr, '\0', sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(FTP_CON_PORT);
	//addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr.S_un.S_addr);
	addr_len = sizeof(sockaddr_in);

	right=bind(con_sock, (sockaddr *)&addr, addr_len);
	if (right == -1) {
		perror("Con Bind");
		return false;
	}

	right=listen(con_sock, 128);
	if (right == -1) {
		perror("Con Listen");
		return false;
	}


	return true;
}

bool Ftp::data_conection() {
	sockaddr_in addr;
	int right;
	int addr_len;

	data_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&addr, '\0', sizeof(sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(FTP_DATA_PORT);
	//addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.S_un.S_addr);
	addr_len = sizeof(sockaddr_in);

	right = bind(data_sock, (sockaddr *)&addr, addr_len);
	if (right == -1) {
		perror("Data Bind");
		return false;
	}

	right = listen(data_sock, 128);
	if (right == -1) {
		perror("Data Listen");
		return false;
	}

	return true;
}

//读取当前路径下的文件名
void Ftp::getCurrentFile(vector<string>& files)
{
	    //文件句柄  
	    intptr_t   hFile = 0;    //x64平台下应用 intptr_t
	    //文件信息  
		 _finddata_t fileinfo;
        string p;
	    if ((hFile = _findfirst(p.assign(cur_path).append("\\*").c_str(), &fileinfo)) != -1)
		    {
		         do
			         {
			            //if (!(fileinfo.attrib &  _A_SUBDIR))
					 if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						                files.push_back(fileinfo.name);
			             
			         }while (_findnext(hFile, &fileinfo) == 0);
			        _findclose(hFile);
			     }
	 }



//发送给客户端下载的文件，成功返回传输的文件行数，失败返回-1
int Ftp::sendFile(SOCKET data_sock, string filename) {
	ifstream file = ifstream(cur_path+"\\"+filename);
	if (!file.is_open()) {
		send(data_sock, "您输入的文件名有误请重新输入", 35, 0);
		cout << "当前目录下没有找到名为" << filename << "  的文件" << endl;
		return -1;
	}
	int i = 0;
	cout << "已传输的文件内容为:" << endl;
	while (!file.eof()) {
		char buf[128]="";
		int len;
		file.getline(buf, sizeof(buf));
		cout << ++i << ":" << buf << endl;
		len = strlen(buf);
		if (len < sizeof(buf)) {
			buf[len++] = '\r';
			buf[len] = '\n';
		}
		send(data_sock, buf, sizeof(buf), 0);
	}
	send(data_sock, "over!", 10, 0);
	file.close();
	return i;
}

