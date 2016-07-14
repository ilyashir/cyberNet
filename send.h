#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include<string>
using namespace std;
#define PortOut 4444
struct Trik
{
    //Конструктор
    SOCKET s;

    Trik(string s_IP)
    {
        start();
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PortOut);
        addr.sin_addr.s_addr = inet_addr(s_IP.c_str());
        connect(s,(SOCKADDR *) & addr, sizeof (addr));
        for(int i=0;i<4;i++)
            vals[i]="0";
    }
    //Деструктор
    ~Trik()
    {
        finish();
    }
    //Создать строки для двух геймпадов
    void strGen()
    {
        pad0="pad 0 "+vals[0]+" "+vals[1]+"\n";
        pad1="pad 1 "+vals[2]+" "+vals[3]+"\n";
    }
    //Отправить обе строки
    void send()
    {
        strGen();
        sendto(s,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr, sizeof (addr));
        sendto(s,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr, sizeof (addr));
        for(int i=0;i<4;i++)
            vals[i]="0";
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
        s=socket(AF_INET,SOCK_STREAM,0);
    }
    //Завершение работы
    void finish()
    {
        closesocket(s);
        WSACleanup();
    }
    //Значения
    string vals[4];
    //Адрес
    sockaddr_in addr;
    //Строки
    string pad0,pad1;
};
