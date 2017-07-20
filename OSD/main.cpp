// ****************SETTINGS******************
#define CAMTYPE                      0
#define CAMID                          0

#define SCREENSIZE                 Size(1366, 768)
#define SCREENPOS                  Point(0, 0)

#define SERVERIP                     "127.0.0.1"
#define SERVERPORT               59666

#define OSDFONT                    FONT_HERSHEY_SIMPLEX
#define OSDFONTCLR              Scalar(0, 255, 100)
#define OSDFONTSIZE             1.0
// ******************************************

#define _CRT_SECURE_NO_WARNINGS
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv_modules.hpp>
#include <opencv2\video\video.hpp>
#include <opencv2\videostab.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\video.hpp>
#include <opencv\highgui.h>
#include <opencv2\core.hpp>
#include <opencv\cv.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "tclient.h"
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <time.h>
#include <windows.h>

using namespace std;
using namespace cv;

const char* cw = "xxxxxxxxxxxxx";
vector <string> telem;
VideoCapture vcap;
Mat img, _img;

double lasttime;
double fps = 0.0;
int c = 0;

bool fff = true;

double millis() {
    SYSTEMTIME st;
    GetSystemTime(&st);
    return st.wMilliseconds / 1000.0;
}

Mat getCam() {
    string url = "http://" + string(SERVERIP) + ":888" + (CAMID == 0 ? "8" : "9") + "/out.jpg";
    vcap.open(url);
    Mat a;
    if (!vcap.isOpened()){
        IplImage* img = cvLoadImage("D:/i1.png", 1);
        a = cvarrToMat(img);
    }
    else{
        vcap.read(a);
    }
    vcap.release();
    resize(a, a, SCREENSIZE);
    return a;
}

void prepareWindow(){
    cw = (CAMID == 0 ? "First camera" : "Second camera");
    namedWindow(cw, WND_PROP_FULLSCREEN);
    setWindowProperty(cw, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    cvMoveWindow(cw, SCREENPOS.x, SCREENPOS.y);
}

vector <string> parse(string s){
    vector <string> res;
    string delimiter = "@";
    string token;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return res;
}

void newTelem(string in){
    telem = parse(in);
}

void putText(string text, Point2f point, CvScalar clr = OSDFONTCLR, double size = OSDFONTSIZE){
    putText(img, text, point, OSDFONT, size, clr, 2);
}

BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT){
        printf("Ctrl-C handled\n");
        fff = false;
        cvDestroyAllWindows();
        exit(0);
    }
    return true;
}

void update1() {
    putText("Altitude: " + telem[T_SONAR], Point2f(60, 50));

    putText("Cam Roll:  " + telem[T_GROLL], Point2f(1100, 50), OSDFONTCLR, 0.9);
    putText("Cam Pitch: " + telem[T_GPITCH], Point2f(1100, 80), OSDFONTCLR, 0.9);
    putText("Cam Yaw:  " + telem[T_GYAW], Point2f(1100, 110), OSDFONTCLR, 0.9);

    putText("Roll:  " + telem[T_ROLL], Point2f(990, 610));
    putText("Pitch: " + telem[T_PITCH], Point2f(990, 640));
    putText("Yaw:  " + telem[T_YAW], Point2f(990, 670));

    putText("Vertical speed: " + telem[T_VSPEED], Point2f(990, 720));
    putText("Ground speed: " + telem[T_GSPEED], Point2f(990, 750));

    int r = (telem[T_LANDGEAR][0] == '0' ? 0 : 240);
    int g = (r == 0 ? 240 : 0);
    int _r = (telem[T_LAND][0] == '0' ? 0 : 240);
    int _g = (_r == 0 ? 240 : 0);

    rectangle(img, CvPoint(60,624), CvPoint(260, 680), CvScalar(0, 180, 80), 4);
    rectangle(img, CvPoint(64,628), CvPoint(256, 676), CvScalar(0, g, r), -1);

    rectangle(img, CvPoint(60,690), CvPoint(260, 747), CvScalar(0, 160, 80), 4);
    rectangle(img, CvPoint(64,694), CvPoint(256, 743), CvScalar(0, _g, _r), -1);

    putText("LAND", Point2f(125, 660), CvScalar(0, 0, 0), 0.9);
    putText("LANDING GEAR", Point2f(80, 727), CvScalar(0, 0, 0), 0.7);
}

void update2() {
    putText("Altitude: " + telem[T_SONAR], Point2f(60, 50));

    putText("Cam Roll:  " + telem[T_GROLL], Point2f(1100, 50), OSDFONTCLR, 0.9);
    putText("Cam Pitch: " + telem[T_GPITCH], Point2f(1100, 80), OSDFONTCLR, 0.9);
    putText("Cam Yaw:  " + telem[T_GYAW], Point2f(1100, 110), OSDFONTCLR, 0.9);

    putText("Roll:  " + telem[T_ROLL], Point2f(990, 610));
    putText("Pitch: " + telem[T_PITCH], Point2f(990, 640));
    putText("Yaw:  " + telem[T_YAW], Point2f(990, 670));

    putText("Vertical speed: " + telem[T_VSPEED], Point2f(990, 720));
    putText("Ground speed: " + telem[T_GSPEED], Point2f(990, 750));

    int r = (telem[T_LANDGEAR][0] == '0' ? 0 : 240);
    int g = (r == 0 ? 240 : 0);
    int _r = (telem[T_LAND][0] == '0' ? 0 : 240);
    int _g = (_r == 0 ? 240 : 0);

    rectangle(img, CvPoint(60,624), CvPoint(260, 680), CvScalar(0, 180, 80), 4);
    rectangle(img, CvPoint(64,628), CvPoint(256, 676), CvScalar(0, g, r), -1);

    rectangle(img, CvPoint(60,690), CvPoint(260, 747), CvScalar(0, 160, 80), 4);
    rectangle(img, CvPoint(64,694), CvPoint(256, 743), CvScalar(0, _g, _r), -1);

    putText("LAND", Point2f(125, 660), CvScalar(0, 0, 0), 0.9);
    putText("LANDING GEAR", Point2f(80, 727), CvScalar(0, 0, 0), 0.7);}

void update(){
    (CAMID == 0 ? update1 : update2)();
    if(c % 50){
        fps = 50.0 / lasttime;
        lasttime = millis();
        c = 0;

        //cout << "FPS: " << fps << endl;
    }
    c++;
}


int main()
{
    //printf("We all will die!!!\n\n");
    std::fill(telem.begin(), telem.end(), "-1111");

    if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
        printf("\nERROR: Could not set control handler");
        return 1;
    }

    bool init = !socket_init(SERVERIP);
    if(init){
        thread serverthr(socket_work, s, newTelem);//, &fff);

        prepareWindow();

        img = getCam();
        lasttime = millis();
        while (fff) {
            if(telem[T_RESERVED] == "666"){
                thread thr(update);
                _img = getCam();
                thr.join();
                try {
                    imshow(cw, img);
                }
                catch (Exception) { cout << 4; }
                img = _img;
            }
            else{
                _img = getCam();
                try {
                    imshow(cw, img);
                }
                catch (Exception) { cout << 4; }
                img = _img;
            }
            if (waitKey(1) == 13) break;
        }
        fff = false;
        cvDestroyAllWindows();
        serverthr.join();
    }
    socket_stop();
    return 0;
}
