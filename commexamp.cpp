#include "trikcomm.h"
int main()
{
    Trik trik("192.168.77.1");
    while(1){
        int vals[4]={228,239,1488,1337};
        trik.set_vals(vals);
        int n=trik.recievemsg();
        cout<<n<<endl;
        trik.sendmsg();
    }

}
