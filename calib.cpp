#include <iostream>
#include <cstdio>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <fstream>
#include <vector>
#include <cmath>
#define PointVal(img, x, y, canal) (*getCvPixelPtr(img, x, y, canal))

using namespace std;

unsigned char* getCvPixelPtr(IplImage* image, int x, int y, int canal)
{
    if(image != NULL)
    {
      return (unsigned char*)(image->imageData + y*image->widthStep + x*image->nChannels + (image->nChannels-canal));
    }
    return NULL;
}

float color1IsColor2(CvScalar color1, CvScalar color2)
{
    float r1 = color1.val[2];
    float g1 = color1.val[1];
    float b1 = color1.val[0];
    float r2 = color2.val[2];
    float g2 = color2.val[1];
    float b2 = color2.val[0];

    float m1 = sqrt(r1*r1 + g1*g1 + b1*b1);
    float m2 = sqrt(r2*r2 + g2*g2 + b2*b2);

    float scalar = (r1*r2 + g1*g2 + b1*b2)/m1/m2;

    return scalar;
}


void sameColor(IplImage* src, IplImage* dst, CvScalar color, double k1 = 0.98, double k2 = 0.4)
{
    for(int x = 0; x < src->width; x++)
    {
        for(int y = 0; y < src->height; y++)
        {
            float similar = color1IsColor2(CV_RGB(PointVal(src, x, y, 1), PointVal(src, x, y, 2), PointVal(src, x, y, 3)), color);
            float light1 = ((int)color.val[2]+color.val[1]+color.val[0])/3.0;
            float light2 = ((int)PointVal(src, x, y, 1) + PointVal(src, x, y, 2) + PointVal(src, x, y, 3))/3.0;

            /*int vr = color.val[2] - PointVal(src, x, y, 1);
            int vg = color.val[1] - PointVal(src, x, y, 2);
            int vb = color.val[0] - PointVal(src, x, y, 3);

            double dist = sqrt(vr*vr+vg*vg+vb*vb);*/

            if(similar > k1 && abs(1-min(light1, light2)/max(light1, light2))<k2)
            {
                PointVal(dst, x, y, 3) = 255;//0;
                PointVal(dst, x, y, 2) = 255;//PointVal(src, x, y, 1);
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
}

bool is_selecting = false;
int firstX = 0, firstY = 0;
int lastX  = 0, lastY  = 0;
vector <CvScalar> flags_colors;
//CvScalar Flags_Color1 = CV_RGB(0, 0, 0);
//CvScalar Flags_Color2 = CV_RGB(0, 0, 0);
//CvScalar Flags_Color3 = CV_RGB(0, 0, 0);

void MyMouse(int event, int X, int Y, int flag, void* param)
{
    IplImage* image = (IplImage*)param;

    int n = 0;

    long long smkR = 0;
    long long smkG = 0;
    long long smkB = 0;

    int temp = 0;

    switch(event)
    {
        case CV_EVENT_LBUTTONDOWN:
            firstX = X;
            firstY = Y;
            is_selecting = true;
        break;

        case CV_EVENT_LBUTTONUP:
            n    = 0;
            smkR = 0;
            smkG = 0;
            smkB = 0;
            for(int x = firstX; x < lastX; x++)
            {
                for(int y = firstY; y < lastY; y++)
                {
                    smkR += PointVal(image, x, y, 1);
                    smkG += PointVal(image, x, y, 2);
                    smkB += PointVal(image, x, y, 3);
                    n++;
                }
            }

            if(n != 0)
            {
                smkR /= n;
                smkG /= n;
                smkB /= n;
                flags_colors.push_back(CV_RGB(smkR, smkG, smkB));
            }
            is_selecting = false;
        break;

        case CV_EVENT_MOUSEMOVE:
            if(lastX < firstX)
            {
                temp   = lastX;
                lastX  = firstX;
                firstX = temp;
            }
            if(lastY < firstY)
            {
                temp   = lastY;
                lastY  = firstY;
                firstY = temp;
            }
            lastX = X;
            lastY = Y;
        break;
    }
}

int main()
{
    /*CvCapture* cap = cvCreateFileCapture("http://admin:admin@192.168.10.239:80/video.mjpg");
    while(true)
    {
        cvShowImage("image", cvQueryFrame(cap));
        cvWaitKey(1);
    }*/

    /*IplImage* image = cvLoadImage("0.png");

    CvPoint p[] = {cvPoint(100, 100), cvPoint(200, 100), cvPoint(200, 250), cvPoint(100, 200)};
    cvFillConvexPoly(image, p, 4, CV_RGB(255, 255, 255));

    cvShowImage("image", image);

    cvWaitKey(0);

    return 0;*/

    CvCapture* capture = cvCreateCameraCapture(1);//cvCaptureFromFile("http://10.23.46.61:8080/?action=streaming.mjpg");
    IplImage*  frame   = NULL;

    IplImage* flags = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
    IplImage* lines = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
    IplImage* bigFlags = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
    double k1 = 0.99;
    double k2 = 1e9;

    cvNamedWindow("camera");
    frame = cvQueryFrame(capture);
    cvSetMouseCallback("camera", MyMouse, frame);
    char c = 0;
    while(c != ' ')
    {
        frame = cvQueryFrame(capture);

        if(is_selecting)
        {
            CvScalar cv = CV_RGB(0, 255, 0);
            cvLine(frame, cvPoint(firstX - 2, firstY - 2), cvPoint(firstX - 2, lastY  + 2),  cv, 1);
            cvLine(frame, cvPoint(firstX - 2, lastY  + 2), cvPoint(lastX  + 2, lastY  + 2),  cv, 1);
            cvLine(frame, cvPoint(lastX  + 2, lastY  + 2), cvPoint(lastX  + 2, firstY - 2),  cv, 1);
            cvLine(frame, cvPoint(lastX  + 2, firstY - 2), cvPoint(firstX - 2, firstY - 2),  cv, 1);
        }

        cvShowImage("camera",  frame);

        c=cvWaitKey(1);
    }




    cout << "Saving..." << endl;

    ofstream out("CalibratedColors");

    //out1 << Flags_Color1.val[2] << " " << Flags_Color1.val[1] << " " << Flags_Color1.val[0] << endl;
    //out2 << Flags_Color2.val[2] << " " << Flags_Color2.val[1] << " " << Flags_Color2.val[0] << endl;
    for(int i=0;i<flags_colors.size();i++)
        out << flags_colors[i].val[2] << " " << flags_colors[i].val[1] << " " << flags_colors[i].val[0] << endl;
    //out << Flags_Color2.val[2] << " " << Flags_Color2.val[1] << " " << Flags_Color2.val[0] << endl;
    //out << Flags_Color3.val[2] << " " << Flags_Color3.val[1] << " " << Flags_Color3.val[0] << endl;
    //out << k1 << " " << k2 << endl;

    cout << "Done." << endl;

    cvReleaseCapture(&capture);

    return 0;
}
