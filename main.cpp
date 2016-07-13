#include <iostream>
#include <Ws2tcpip.h>
#include <string>
using namespace std;

void sendAnswer(int n, SOCKET sct){
	char *c = new char;
	itoa(n, c, 10);
    string str = "pad 0 " + (string)c + " 0\n";
    send(sct, &str[0], str.size(), 0);
}

int main(){
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	int port_in = 8888;
	int port_out = 4444;
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("10.23.47.110");
	addr.sin_port = htons(port_in);
    SOCKET sct_in = socket(AF_INET, SOCK_STREAM, 0);
    connect(sct_in,(SOCKADDR *) &addr, sizeof(addr));
	addr.sin_port = htons(port_out);
	SOCKET sct_out = socket(AF_INET, SOCK_STREAM, 0);
	connect(sct_out, (SOCKADDR *) &addr, sizeof(addr));

	char *buf = new char[11];
	int n = 0;
	string s = "keepalive";
	while(true){
		send(sct_in, &s[0], s.size(), 0);
		n = recv(sct_in, buf, 11, 0);
		cout << n << endl;
		if(buf[9] == '3' && buf[10] == '2'){
			cout << "received" << endl;
			break;
		}
		Sleep(100);
	}
	sendAnswer(4, sct_out);

    closesocket(sct_in);
	closesocket(sct_out);
    WSACleanup();
	system("pause");
    return 0;
}