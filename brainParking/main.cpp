#include "header.h"
#include <Ws2tcpip.h>

int* getAvailablePlaces(IplImage *image){
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	cvFindContours(image, storage, &contours);
	CvRect rect;
	double s = 0;
	double p = 0;
	int x;
	int y;
	int arr[] = {0, 0, 0, 0, 0, 0};
	for(CvSeq *seq = contours; seq != 0; seq = seq->h_next){
		s = cvContourArea(seq);
		p = cvArcLength(seq);
		if(abs(p*p/s) < 4*3.14 + 2 && s > 90){
			rect = cvBoundingRect(seq);
			x = rect.x + rect.width/2;
			y = rect.y + rect.height/2;
			if(30 < y && y < 50){
				if(150 < x && x < 180){
					arr[1] = 1;
				}
				if(200 < x && x < 220){
					arr[3] = 1;
				}
				if(240 < x && x < 270){
					arr[5] = 1;
				}
			}
			if(190 < y && y < 210){
				if(150 < x && x < 180){
					arr[0] = 1;
				}
				if(200 < x && x < 220){
					arr[2] = 1;
				}
				if(240 < x && x < 270){
					arr[4] = 1;
				}
			}
		}
	}
	return arr;
}

int getBestPlace(int *arr){
	int brr[] = {6, 5, 4, 3, 2, 1};
	int max = 0;
	int index = -1;
	for(int i = 0; i<6; i++){
		if(arr[i]*brr[i] > max){
			max = arr[i]*brr[i];
			index = i;
		}
	}
	return index + 1;
}

void sendAnswer(SOCKET sct, int place){
	char *c = new char;
	itoa(place, c, 10);
	std::string s = "pad 0 " + (std::string)c + " 0\n";
    send(sct, &s[0], s.size(), 0);
	delete c;
}

int main(){
	//подключенные автомобили
	int num = 1;
	char* ip[] = {"0.0.0.0"};

	//получение сокетов
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	int port_in = 8888;
	int port_out = 4444;
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	SOCKET *sct_in = new SOCKET[num];
	SOCKET *sct_out = new SOCKET[num];
	for(int i = 0; i<num; i++){
		addr.sin_addr.s_addr = inet_addr(ip[i]);
		addr.sin_port = htons(port_in);
		sct_in[i] = socket(AF_INET, SOCK_STREAM, 0);
		connect(sct_in[i],(SOCKADDR *) &addr, sizeof(addr));
		addr.sin_port = htons(port_out);
		sct_out[i] = socket(AF_INET, SOCK_STREAM, 0);
		connect(sct_out[i], (SOCKADDR *) &addr, sizeof(addr));
	}
	char *buf = new char[11];
	std::string s = "keepalive";
	int n = 0;

	//получение изображения
	CvCapture *capture = cvCreateFileCapture("http://192.168.77.1:8080/?action=stream.mjpg");
	assert(capture);
	cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);
	IplImage *frame = 0;
	IplImage *image = 0;
	while(true){
		frame = cvQueryFrame(capture);
		image = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvConvertImage(frame, image, CV_YUV2BGR);
		cvConvertImage(image, image, CV_BGR2GRAY);
		cvSmooth(image, image, CV_GAUSSIAN, 3);
		cvThreshold(image, image, 120, 255, CV_THRESH_BINARY);
		cvErode(image, image);
		cvShowImage("capture", image);
		/* проверка на новые сообщения */
		for(int i = 0; i<num; i++){
			send(sct_in[i], &s[0], s.size(), 0);
			n = recv(sct_in[i], buf, 11, 0);
			if(n <= 0){
				std::cout << "error" << std::endl;
			}
			if(buf != "9:keepalive"){
				std::cout << buf << std::endl;
				n = getBestPlace(getAvailablePlaces(image));
				std::cout << n << std::endl;
				sendAnswer(sct_out[i], n);
			}
		}
		/* проверка на новые сообщения */
		if(cvWaitKey(50) == 32){
			break;
		}
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("capture");

	//закрытие сокетов
	for(int i = 0; i<num; i++){
		closesocket(sct_in[i]);
		closesocket(sct_out[i]);
	}
	delete sct_in;
	delete sct_out;
	WSACleanup();
	return 0;
}