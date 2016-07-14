#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include<string>
using namespace std;
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
        buf = new char[13];
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
    }
    //Отправить обе строки
    void sendmsg()
    {
        strGen();
        sendto(s_out,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
        sendto(s_out,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
        for(int i=0;i<4;i++)
            vals[i]="0";
    }
    int recievemsg()
    {
        int n;
        string str="keepalive";
        send(s_in, &str[0], str.size(), 0);
		n=recv(s_in, buf, 13, 0);
		return n;
    }
    //Записать строки в произвольном порядке
    void set_str(string s1, int v1, string s2="", int v2=-1, string s3="", int v3=-1, string s4="", int v4=-1)
    {
        vals[v1]=s1;
        if(v2!=-1)
            vals[v2]=s2;
        if(v3!=-1)
            vals[v3]=s3;
        if(v4!=-1)
            vals[v4]=s4;
    }
    //Записать строки последовательно
    void set_str(int n, string s1, string s2="", string s3="", string s4="")
    {
        switch(n)
        {
        case 4:
            vals[3]=s4;
        case 3:
            vals[2]=s3;
        case 2:
            vals[1]=s2;
        case 1:
            vals[0]=s1;
        }
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
