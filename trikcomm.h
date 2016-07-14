#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;
#define BUFF 20
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
struct Trik
{
    //Конструктор
    SOCKET s_out;
    SOCKET s_in;
    char *buf;
    Trik(string s_IP)
    {
        static bool flag = true;
        if(flag)
            start();
        flag=false;
        //Конструируем отправку
        s_out = socket(AF_INET, SOCK_STREAM, 0);
        addr_out.sin_family = AF_INET;
        addr_out.sin_port = htons(4444);
        addr_out.sin_addr.s_addr = inet_addr(s_IP.c_str());
        connect(s_out,(SOCKADDR *) & addr_out, sizeof (addr_out));
        for(int i=0;i<4;i++)
            vals[i]="0";
        //Конструируем прием
        s_in=socket(AF_INET,SOCK_STREAM,0);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(8888);
        addr_in.sin_addr.s_addr = inet_addr(s_IP.c_str());
        connect(s_in,(SOCKADDR *) &addr_in, sizeof(addr_in));
        alive();
        buf = new char[BUFF];
    }
    //Деструктор
    ~Trik()
    {
        static bool flag = true;
        if(flag)
            finish();
        flag=false;
    }
    //Создать строки для двух геймпадов
    void strGen()
    {
        pad0="pad 0 "+vals[0]+" "+vals[1]+"\n";
        pad1="pad 1 "+vals[2]+" "+vals[3]+"\n";
        alive();
    }
    //Отправить обе строки
    void sendmsg()
    {
        alive();
        strGen();
        sendto(s_out,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
        sendto(s_out,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
        for(int i=0;i<4;i++)
            vals[i]="0";
        alive();
    }
    void alive()
    {
        string str="keepalive";
        send(s_in, &str[0], str.size(), 0);
    }
    int recievemsg()
    {
        alive();
        int n;
        n=recv(s_in, buf, BUFF, 0);
        int l=0;
        for(int i=0;i<20&&buf[i]!=':';i++)
            l=l*10+buf[i]-'0';
        int v=0;
        for(int i=(n-l)+7;i<n;i++)
            v=v*10+buf[i]-'0';
        alive();
		return v;
    }
    void set_vals(int* val)
    {
        for(int i=0;i<4;i++)
            vals[i]=SSTR(val[i]);
        alive();
    }
    //Запуск связи
    void start()
    {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    //Завершение работы
    void finish()
    {
        closesocket(s_out);
        closesocket(s_in);
        WSACleanup();
    }
    //Значения
    string vals[4];
    //Адрес
    sockaddr_in addr_out;
    sockaddr_in addr_in;
    //Строки
    string pad0,pad1;
};
