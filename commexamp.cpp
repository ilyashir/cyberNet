#include "trikcomm.h"
int main()
{
    //int n=20;
    //string str="17:print: 1234567890";
    //cout<<str.substr(n-3>9?2:1,8);
    //return 0;
    Trik trik("192.168.77.1");
    while(1){
        Sleep(1000);
        int vals[4]={228,239,1488,1337};
        trik.set_vals(vals);
        int n=trik.recievemsg(4);
        cout<<n<<endl;
        trik.sendmsg();
    }
}
