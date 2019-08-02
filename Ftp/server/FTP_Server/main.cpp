
#include "Server.h"

int main() {
	Server FtpServer;
	FtpServer.run();
	WSACleanup();
	return 0;
}