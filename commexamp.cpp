#include "trikcomm.h"
int main()
{
    Trik trik("192.168.77.1");
    cout<<'.';
    while(1){
        trik.set_str(4,"1","2","3","4");
        trik.recievemsg();
        cout<<trik.buf<<endl;
        trik.sendmsg();
    }

}
