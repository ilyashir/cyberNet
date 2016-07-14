#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <thread>
#include <ctime>
using namespace std;
#define BUFF 120
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
class Trik
{
private:
    SOCKET s_out;
    SOCKET s_in;
    sockaddr_in addr_out;
    sockaddr_in addr_in;
    char *buf;
    thread aliver;
    string pad0,pad1;
    string vals[4];
    //Создать строки для двух геймпадов
    void strGen()
    {
        pad0="pad 0 "+vals[0]+" "+vals[1]+"\n";
        pad1="pad 1 "+vals[2]+" "+vals[3]+"\n";

    }
    //Запуск связи
    int start()
    {
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData);
    }
    //Завершение работы
    void finish()
    {
        closesocket(s_out);
        closesocket(s_in);
        WSACleanup();
    }
public:
    bool active;
    void alive()
    {
        string str="keepalive";
        while(active){
            int t=clock();
            send(s_in, &str[0], str.size(), 0);
            while(clock()-t<100);
        }
    }
    Trik(string s_IP)
    {
        active=false;
        static bool flag = true;
        int err=0;
        if(flag)
            err|=start();
        flag=false;
        //Конструируем отправку
        s_out = socket(AF_INET, SOCK_STREAM, 0);
        addr_out.sin_family = AF_INET;
        addr_out.sin_port = htons(4444);
        addr_out.sin_addr.s_addr = inet_addr(s_IP.c_str());
        err|=connect(s_out,(SOCKADDR *) & addr_out, sizeof (addr_out));
        //Конструируем прием
        s_in=socket(AF_INET,SOCK_STREAM,0);
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(8888);
        addr_in.sin_addr.s_addr = inet_addr(s_IP.c_str());
        err|=connect(s_in,(SOCKADDR *) &addr_in, sizeof(addr_in));
        //Инициализируем переменные
        if(err==0)
            active=true;
        aliver=thread([=]{alive();});
        for(int i=0;i<4;i++)
            vals[i]="0";
        buf = new char[BUFF];
    }
    //Деструктор
    ~Trik()
    {
        delete [] buf;
        active=false;
        aliver.join();
        static bool flag = true;
        if(flag)
            finish();
        flag=false;
    }
    //Отправить обе строки
    void sendmsg()
    {
        strGen();
        if(active)
        {
            sendto(s_out,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
            sendto(s_out,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr_out, sizeof (addr_out));
        }
    }
    void set_vals(int* val)
    {
        for(int i=0;i<4;i++)
            vals[i]=SSTR(val[i]);
    }
    int recievemsg(int len)
    {

        if(!active||len>100)
            return -1;
        int n=recv(s_in, buf, (size_t)(len+(len>10?9:10)), 0),l=0,v=0;
        if(buf[1]==':')
            if(n!=buf[0]-'0'+3)
                return -2;
        else;else if(buf[2]==':')
            if(n!=buf[0]*10+buf[1]-'0'*11+3)
                return -2;
        else; else return -2;
        if(string(buf).substr(n-3>9?2:1,8)!=":print: ")
            return -1;
        for(int i=0;i<BUFF&&buf[i]!=':';i++)
            l=l*10+buf[i]-'0';
        for(int i=(n-l)+7;i<n;i++)
            v=v*10+buf[i]-'0';
		return v;
    }

};
