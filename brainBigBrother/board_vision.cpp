#include <iostream>
#include <fstream>
#include <string>
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include <ctime>
#include <windows.h>
#include "trikcomm.h"
using namespace std;
#define PointVal(image, x, y, channel) (*getCvPixelPtr(image, x, y, channel))

const double PI = 3.1415926535897932384626433832795;
const int scale=2;

//Получение значений пикселей
unsigned char* getCvPixelPtr(IplImage* image, int x, int y, int channel)
{
    return (unsigned char*)(image->imageData + y*image->widthStep + x*image->nChannels + (image->nChannels-channel));
}
//Считываем закалиброванный цвет из файла
CvScalar getColor(int num)
{
    static vector <CvScalar> colors;
    static ifstream in("CalibratedColors");
    while(colors.size()<=num)
    {
        int r,g,b;
        in>>r>>g>>b;
        colors.push_back(CV_RGB(r,g,b));
    }
    return colors[num];
}

using namespace std;
//Сравниваем цвета
float color1IsColor2(CvScalar color1, CvScalar color2)
{
    static double r1,g1,b1,r2,b2,g2;
    r1 = color1.val[2];
    g1 = color1.val[1];
    b1 = color1.val[0];
    r2 = color2.val[2];
    g2 = color2.val[1];
    b2 = color2.val[0];
    return (r1*r2 + g1*g2 + b1*b2)/sqrt(r1*r1 + g1*g1 + b1*b1)/sqrt(r2*r2 + g2*g2 + b2*b2);
}
//Сравниваем цвета на всем изображении с эталонным
IplImage* sameColor(IplImage* src, IplImage* dst, int numColor = 0, double k1 = 0.98)
{
    CvScalar color=getColor(numColor);
    for(int x = 0; x < src->width; x++)
    {
        for(int y = 0; y < src->height; y++)
        {
            float similar = color1IsColor2(CV_RGB(PointVal(src, x, y, 1), PointVal(src, x, y, 2), PointVal(src, x, y, 3)), color);
            //float light1 = ((int)color.val[2]+color.val[1]+color.val[0])/3.0;
            //float light2 = ((int)PointVal(src, x, y, 1) + PointVal(src, x, y, 2) + PointVal(src, x, y, 3))/3.0;
            /*int vr = color.val[2] - PointVal(src, x, y, 1);
            int vg = color.val[1] - PointVal(src, x, y, 2);
            int vb = color.val[0] - PointVal(src, x, y, 3);

            double dist = sqrt(vr*vr+vg*vg+vb*vb);*/

            if(similar > k1)
            {
                //if(numColor == 0&&y>src->height/2)
                //    cout<<color.val[0]<<'-'<<(int)PointVal(src, x, y, 1)<<' '<<color.val[1]<<'-'<<(int)PointVal(src, x, y, 2)<<' '<<color.val[2]<<'-'<<(int)PointVal(src, x, y, 3)<<' '<<similar<<endl;

                //PointVal(dst, x, y, 3) = PointVal(src, x, y, 3);//0;
                //PointVal(dst, x, y, 2) = PointVal(src, x, y, 2);//0;
                //PointVal(dst, x, y, 1) = PointVal(src, x, y, 1);//0;
                PointVal(dst, x, y, 2) = 255;//PointVal(src, x, y, 1);
                PointVal(dst, x, y, 3) = 255;//PointVal(src, x, y, 1);
                PointVal(dst, x, y, 1) = 255;//(255 * 6 + PointVal(src, x, y, 1) * 4) / 10;
            }
            else
            {
                PointVal(dst, x, y, 1) = 0;//PointVal(frame, x, y, 1) / 4;
                PointVal(dst, x, y, 2) = 0;//PointVal(frame, x, y, 2) / 4;
                PointVal(dst, x, y, 3) = 0;//PointVal(frame, x, y, 3) / 4;
            }
        }
    }
    return dst;
}
//Положение робота
struct Robot
{
    CvPoint left_point,right_point,center;
    double ang, radius;
    void calculate(CvPoint l, CvPoint r)
    {
        left_point=l;
        right_point=r;
        center=cvPoint((l.x+r.x)/2,(l.y+r.y)/2);
        radius=_hypot(r.x-l.x,r.y-l.y)*1.5;
        ang=atan2(r.x-l.x,r.y-l.y);
    }
};
//Компонента связности
struct Comp
{
    int size;
    int perimeter;
    CvPoint center;
    int num;
    Comp()
    {
        size=0;
        perimeter=0;
        center=cvPoint(0,0);
        num=-1;
    }
};
//Все компоненты на изображении
struct Comps
{
    vector<Comp> comps;
    vector<vector<int> > windowArray;
    Comp maxComp;
    Comps()
    {

    }
    Comps(IplImage *image)
    {
        int sW=image->width;
        int sH=image->height;
        windowArray.resize(sW,vector<int>(sH,-1));
        comps.resize(1,Comp());
        static queue <CvPoint> q;
        int compNum=0;
        for (int i = 0; i < sW; i++)
        {
            for (int n = 0; n < sH; n++)
            {
                if ((unsigned char)PointVal(image,i,n,1)==255 && windowArray[i][n] == -1)
                {
                    static long long sum,sumX,sumY;
                    sum = 1;
                    sumX = i;
                    sumY = n;
                    comps[compNum].size=1;
                    comps[compNum].perimeter=1;
                    windowArray[i][n] = compNum;

                    q.push(cvPoint (i, n));
                    while (q.size()>0)
                    {
                        comps[compNum].size++;
                        CvPoint p = q.front();
                        sum++;
                        sumX+=p.x;
                        sumY+=p.y;
                        q.pop();
                        static int dp;
                        dp=0;
                        static int iter;
                        for(iter=0;iter<4;iter++){
                            static int dx,dy;
                            dx=iter<2?-1:1;
                            dy=iter%2==0?-1:1;
                            CvPoint t = cvPoint (p.x + dx, p.y + dy);
                            if ((dx == 0 && dy == 0) || t.x < 0 || t.y < 0 || t.x >= sW || t.y >= sH)
                                continue;
                            if((unsigned char)PointVal(image,t.x,t.y,1)!=255 || windowArray[t.x][t.y] != -1)
                                continue;
                            dp++;
                            q.push(t);
                            windowArray[t.x][t.y] = compNum;

                        }
                        comps[compNum].perimeter+=dp==8?0:1;
                    }
                    static float cenX;cenX=sumX/sum;
                    static float cenY;cenY=sumY/sum;
                    comps[compNum].center=cvPoint((int)cenX,(int)cenY);
                    comps[compNum].num=compNum;
                    if(comps[compNum].size>maxComp.size)
                    {
                        maxComp.size=comps[compNum].size;
                        maxComp.center=comps[compNum].center;
                        maxComp.perimeter=comps[compNum].perimeter;
                        maxComp.num=comps[compNum].num;
                    }
                    comps.push_back(Comp());
                    compNum++;
                }
            }
        }
    }
};
void ProcessBoard(IplImage* frame, IplImage* final_b, Comps& comps_o, Comps& comps_y, Comps& comps_board, Robot& robot)
{
    static IplImage* image = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* grey = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,1);
    static IplImage* yellow = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* orange = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* red = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* green = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* blue = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* text1 = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* text2 = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,1);
    static IplImage* text3 = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* text4 = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    static IplImage* text5 = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
    cvResize(frame,image);

    //Делаем контрастное чб изображение
    for(int i=0;i<image->width;i++)
        for(int j=0;j<image->height;j++)
            PointVal(grey,i,j,0)=min(PointVal(image,i,j,1),min(PointVal(image,i,j,2),PointVal(image,i,j,3)));
    //Бинаризуем контрастные изображения
    cvThreshold(grey,grey,115,255,CV_THRESH_BINARY);
    //Определяем доску компонентой
    comps_board = Comps(grey);
    //Убираем все, что этой компоненте не принадлежит
    for(int i=0;i<image->width;i++)
    {
        for(int j=0;j<image->height&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;j++)
            cvCircle(image,cvPoint(i,j),0,CV_RGB(255,255,255),CV_FILLED);
        for(int j=image->height-1;j>=0&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;j--)
            cvCircle(image,cvPoint(i,j),0,CV_RGB(255,255,255),CV_FILLED);
    }
    for(int j=0;j<image->height;j++)
    {
        for(int i=0;i<image->width&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;i++)
            cvCircle(image,cvPoint(i,j),0,CV_RGB(255,255,255),CV_FILLED);
        for(int i=image->width-1;i>=0&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;i--)
            cvCircle(image,cvPoint(i,j),0,CV_RGB(255,255,255),CV_FILLED);
    }
    cvCopy(image, text1);
    //Алгоритмом Лапласа ищем контрастные участки
    cvLaplace(text1,text1);
    for(int i=0;i<text2->width;i++)
        for(int j=0;j<text2->height;j++)
            PointVal(text2,i,j,1)=0;
    //Увеличиваем контрастность
    for(int i=0;i<text1->width;i++)
        for(int j=0;j<text1->height;j++)
            PointVal(text2,i,j,1)=max(PointVal(text1,i,j,1),max(PointVal(text1,i,j,2),PointVal(text1,i,j,3)));
    cvThreshold(text2,text2,30,255,CV_THRESH_BINARY_INV);
    //Убираем рябь по краю доски
    for(int i=0;i<text2->width;i++)
    {
        for(int j=0;j<text2->height&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;j++)
            cvCircle(text2,cvPoint(i,j),3,CV_RGB(255,255,255),CV_FILLED);
        for(int j=text2->height-1;j>=0&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;j--)
            cvCircle(text2,cvPoint(i,j),3,CV_RGB(255,255,255),CV_FILLED);
    }
    for(int j=0;j<text2->height;j++)
    {
        for(int i=0;i<text2->width&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;i++)
            cvCircle(text2,cvPoint(i,j),3,CV_RGB(255,255,255),CV_FILLED);
        for(int i=text2->width-1;i>=0&&comps_board.windowArray[i][j]!=comps_board.maxComp.num;i--)
            cvCircle(text2,cvPoint(i,j),3,CV_RGB(255,255,255),CV_FILLED);
    }

    //Ищем желтую и оранжевую метки
    comps_y = Comps(sameColor(image,yellow));
    comps_o = Comps(sameColor(image,orange,1));
    //Разделяем на зеленый - не зеленый
    for(int x=0;x<image->width;x++)
        for(int y=0;y<image->height;y++)
        {
            CvPoint p=cvPoint(x,y);
            if(PointVal(text2,x,y,1)==0)
            {
                float pow=0;int ind=3;
                for(int i=0;i<=0;i++)
                    for(int j=0;j<=0;j++)
                {
                    CvPoint p=cvPoint(x+i,y+j);
                    if(p.x<0||p.x>=image->width||p.y<0||p.y>=image->height)
                        continue;
                    double
                    r=PointVal(image, p.x, p.y, 1),
                    g=PointVal(image, p.x, p.y, 2),
                    b=PointVal(image, p.x, p.y, 3),
                    k=max(max(PointVal(image, p.x, p.y, 1), PointVal(image, p.x, p.y, 2)), PointVal(image, p.x, p.y, 3));//cout<<r<<' '<<g<<' '<<k<<endl;
                    if(r==k)
                        ind=0;
                    else if(g-r>b-g)
                        ind=1;
                    else
                        ind=2;
                }
                switch(ind)
                {
                case 0:
                    cvCircle(text3,p,0,CV_RGB(255,0,0));
                    //if(pow>0.7)
                        break;
                case 1:
                    cvCircle(text3,p,0,CV_RGB(0,255,0));
                    //if(pow>0.98)
                        break;
                case 2:
                    cvCircle(text3,p,0,CV_RGB(0,0,255));
                    //if(pow>0.98)
                        break;
                default:
                    cvCircle(text3,p,0,CV_RGB(0,0,0));
                }
            }
            else
                cvCircle(text3,p,0,CV_RGB(255,255,255));
        }
    robot.calculate(comps_y.maxComp.center,comps_o.maxComp.center);
    //cout<<robot.center.x<<' '<<robot.center.y<<endl;
    //Игнорируем робота
    cvCircle(text3,robot.center,robot.radius,CV_RGB(255,255,255),CV_FILLED);
    cvCopy(text3,text4);
    //Убираем все, что не выделено
    for(int i=0;i<text3->width;i++)
    {
        for(int j=0;j<text3->height&&(PointVal(text3,i,j,2)!=0||PointVal(text3,i,j,3)!=0);j++)
            PointVal(text4,i,j,1)=PointVal(text4,i,j,2)=PointVal(text4,i,j,3)=255;
        for(int j=text3->height-1;j>=0&&(PointVal(text3,i,j,2)!=0||PointVal(text3,i,j,3)!=0);j--)
            PointVal(text4,i,j,1)=PointVal(text4,i,j,2)=PointVal(text4,i,j,3)=255;
    }
    for(int j=0;j<text3->height;j++)
    {
        for(int i=0;i<text3->width&&(PointVal(text3,i,j,2)!=0||PointVal(text3,i,j,3)!=0);i++)
            PointVal(text4,i,j,1)=PointVal(text4,i,j,2)=PointVal(text4,i,j,3)=255;
        for(int i=text3->width-1;i>=0&&(PointVal(text3,i,j,2)!=0||PointVal(text3,i,j,3)!=0);i--)
            PointVal(text4,i,j,1)=PointVal(text4,i,j,2)=PointVal(text4,i,j,3)=255;
    }
    const int speed=30;
    //Создаем финальное изображение
    static IplImage* board = NULL;
    if(board==NULL)
    {
        board = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,3);
        cvCopy(text4,board);
    }
    //Убираем помехи
    for(int i=0;i<board->width;i++)
        for(int j=0;j<board->height;j++)
    {
        if(_hypot(i-robot.center.x,j-robot.center.y)<=robot.radius/2)
            cvCircle(board,cvPoint(i,j),0,CV_RGB(255,255,255));
        if(_hypot(i-robot.center.x,j-robot.center.y)<=robot.radius)
            continue;
        for(int k=0;k<board->nChannels;k++)
        {
            int a=PointVal(board,i,j,k);
            if(PointVal(text4,i,j,k)==0)
                PointVal(board,i,j,k)=max(a-speed,0);
            if(PointVal(text4,i,j,k)==255)
                PointVal(board,i,j,k)=min(a+speed,255);
        }
    }
    cvThreshold(board,text5,50,255,CV_THRESH_BINARY);
    cvConvertImage(text5,final_b);
    //cvShowImage("yellow",yellow);
    //cvShowImage("orange",orange);
    cvThreshold(final_b,final_b,254,255,CV_THRESH_BINARY);
}
int main()
{
    Trik trik("192.168.77.1");
    Robot robot;
    Comps comps_y,comps_o,comps_board;
    cout<<"W8 ";
    IplImage*  frame   = NULL;
    char c=0;
    CvCapture* capture = cvCreateCameraCapture(1);
    while(c != 13)
    {
        float fps=31;
        /*while(fps>30)
        {
            int time=clock();
            fps=1000.0/(clock()-time);
        }*/

        frame = cvQueryFrame(capture);
        static IplImage* final_b = cvCreateImage(cvSize(frame->width/scale,frame->height/scale),IPL_DEPTH_8U,1);
        ProcessBoard(frame,final_b,comps_o,comps_y,comps_board,robot);


        //Отрисовываем робота
        cvCircle(frame,cvPoint(robot.center.x*2,robot.center.y*2),robot.radius*2,CV_RGB(255,0,0));
        cvCircle(frame,cvPoint(robot.center.x*2,robot.center.y*2),3,CV_RGB(255,0,0),-1);
        cvCircle(frame,cvPoint(robot.left_point.x*2,robot.left_point.y*2),3,CV_RGB(255,255,0),-1);
        cvCircle(frame,cvPoint(robot.right_point.x*2,robot.right_point.y*2),3,CV_RGB(255,128,0),-1);
        cvShowImage("frame",frame);
        cvShowImage("final",final_b);
        int d=2000,x=-1,y=-1;
        for(int i=0;i<final_b->width;i++)
            for(int j=0;j<final_b->height;j++)
                if(PointVal(final_b,i,j,1)==0)
                    if(_hypot(i-robot.center.x,j-robot.center.y)<d)
                    {
                        d=_hypot(i-robot.center.x,j-robot.center.y);
                        x=i;
                        y=j;
                    }
        double ang=atan2(x-robot.center.x,y-robot.center.y);
        int deg=(ang-robot.ang-PI/2)/PI*180;
        int dist=_hypot(x-robot.center.x,y-robot.center.y);
        if(deg<0)
            deg+=360;
        trik.sendmsg(deg,dist);
        c=cvWaitKey(1);

    }
    cvReleaseCapture(&capture);

}
