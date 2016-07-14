#include <iostream>
#include <Ws2tcpip.h>
#include <string>
using namespace std;
int main(){
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	int port_in = 8888;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("10.23.46.240");
	addr.sin_port = htons(port_in);
    SOCKET sct_in = socket(AF_INET, SOCK_STREAM, 0);
    connect(sct_in,(SOCKADDR *) &addr, sizeof(addr));
	char *buf = new char[13];
	string s = "keepalive";
	while(true){
        int n = 0;
		send(sct_in, &s[0], s.size(), 0);
		n = recv(sct_in, buf, 13, 0);
		cout << buf << endl;
		Sleep(1);
	}
    closesocket(sct_in);
    WSACleanup();
	system("pause");
    return 0;
}
