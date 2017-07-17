// ****************SETTINGS******************
#define SCREENSIZE          Size(1366, 768)
#define SCREENPOS           Point(0, 0)

#define SERVERIP             "127.0.0.1"
#define SERVERPORT         59666

#define CAMID                  0
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
#include <vector>
#include <thread>
#include <time.h>
#include "tclient.h"
#include <windows.h>

using namespace std;
using namespace cv;

const char* cw = "xxxxxxxxxxxxx";
VideoCapture vcap;
Mat img, _img;

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

void update1() {
    putText(img, to_string(123.123456), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
}

void update2() {
    putText(img, to_string(123.123456), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
}

void update(){
    (CAMID == 0 ? update1 : update2)();
}

void prepareWindow(){
    cw = (CAMID == 0 ? "First camera" : "Second camera");
    namedWindow(cw, WND_PROP_FULLSCREEN);
    setWindowProperty(cw, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    cvMoveWindow(cw, SCREENPOS.x, SCREENPOS.y);
}

void newTelem(vector <string>){

}

int main()
{
    socket_init(SERVERIP);
    thread serverthr(socket_work, s, newTelem);

    prepareWindow();

    img = getCam();
    while (true) {
        thread thr(update2);
        _img = getCam();
        thr.join();
        try {
            imshow(cw, img);
        }
        catch (Exception) {}
        img = _img;

        if (waitKey(1) == 13) break;
    }
    cvDestroyAllWindows();
    serverthr.join();
    socket_stop();
    return 0;
}
