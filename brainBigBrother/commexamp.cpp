#include "trikcomm.h"
int main()
{
    //Коннектим
    Trik trik("192.168.77.1");
    while(1){
        //Посылаем
        trik.sendmsg(228,239,1488,1337);
        //Принимаем
        cout<<trik.recievemsg(4)<<endl;
    }
}
