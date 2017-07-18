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

void putText(string text, Point2f point){
    putText(img, text, point, OSDFONT, OSDFONTSIZE, OSDFONTCLR, 1);
}

BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT){
        printf("Ctrl-C handled\n");
        fff = false;
    }
    return true;
}

void update1() {
    putText("Altitude: " + telem[T_ALT], Point2f(10, 30));
    putText("FPS: " + to_string(fps), Point2f(10, 760));
}

void update2() {
    putText("Altitude: " + telem[T_ALT], Point2f(10, 30));
    putText("FPS: " + to_string(fps), Point2f(10, 760));
}

void update(){
    (CAMID == 0 ? update1 : update2)();
    if(c % 50){
        fps = 50.0 / lasttime;
        lasttime = millis();
        c = 0;

        cout << "FPS: " << fps << endl;
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
        while (true) {
            cout<<telem[T_RESERVED];
            if(telem[T_RESERVED] == "666"){
                thread thr(update);
                _img = getCam();
                thr.join();
                try {
                    imshow(cw, img);
                }
                catch (Exception) {}
                img = _img;

                if (waitKey(1) == 13) break;
                fff = true;
            }
        }
        fff = false;
        cvDestroyAllWindows();
        serverthr.join();
    }
    socket_stop();
    return 0;
}
