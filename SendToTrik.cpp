#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include<string>
using namespace std;
#define PortOut 4444
SOCKET s;

/*
//Класс передачи данных на Трику
*/
class Trik
{
public:
    //Конструктор класса
    Trik(char* IP);
    //Отправить строку
    void send(string data,int lenthOfData);
    //Отправить строку pad0 или pad1
    void send(int pad);
    //Отправить обе строки
    void send();
    //Создать строку для двух параметров на геймпад
    void strGen(int padNum,string x,string y);
    //Создать строки для двух геймпадов
    void strGen(string a,string b,string c,string d);
    //пятый параметр
    void strGen(string a,string b,string c,string d,string e);
private:
    //Адрес
    sockaddr_in addr;
    //Строки
    string pad0,pad1;
};
//Конструктор класса
Trik::Trik(char* IP)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PortOut);
    addr.sin_addr.s_addr = inet_addr(IP);
    connect(s,(SOCKADDR *) & addr, sizeof (addr));
}
//Отправить строку
void Trik::send(string data,int lenthOfData)
{
    sendto(s,&data[0],lenthOfData,0,(SOCKADDR *) & addr, sizeof (addr));
}
//Отправить строку pad0 или pad1
void Trik::send(int pad)
{
    if(pad==0)
    {
        sendto(s,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr, sizeof (addr));
    }
    else
    {
        sendto(s,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr, sizeof (addr));
    }
}
//Отправить обе строки
void Trik::send()
{
    sendto(s,&pad0[0],pad0.size(),0,(SOCKADDR *) & addr, sizeof (addr));
    sendto(s,&pad1[0],pad1.size(),0,(SOCKADDR *) & addr, sizeof (addr));
}
//Создать строку для двух параметров
void Trik::strGen(int padNum,string x,string y)
{
    if(padNum==0)
    {
        pad0="pad 0 "+x+" "+y+"\n";
        cout<<pad0<<endl;
    }
    else
    {
        pad1="pad 1 "+x+" "+y+"\n";
        cout<<pad1<<endl;
    }
}
//Создать строки для двух геймпадов
void Trik::strGen(string a,string b,string c,string d)
{
    pad0="pad 0 "+a+" "+b+"\n";
    pad1="pad 1 "+c+" "+d+"\n";
}
void Trik::strGen(string a,string b,string c,string d,string e)
{
    cout<<strlen(&e[0])<<endl;
    switch (strlen(&e[0]))
    {
    case 1:
        e="00"+e;
        break;
    case 2:
        e="0"+e;
        break;
    }
    pad0="pad 0 "+a+e+" "+b+"\n";
    pad1="pad 1 "+c+" "+d+"\n";
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
int main()
{
    start();
    Trik trik1("10.23.46.128");
    while(true)
    {
        string a,b,c,d,e;
        cin>>a>>b>>c>>d>>e;
        trik1.strGen(a,b,c,d,e);
        trik1.send();
    }
    finish();
    return 0;
}
