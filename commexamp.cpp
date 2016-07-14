#include "trikcomm.h"
int main()
{
    Trik trik("192.168.77.11");
    trik.set_str(4,"1","2","3","4");
    trik.sendmsg();
    int n=1;
    while(n!=0)
        n=trik.recievemsg();
    cout<<trik.buf;
}
