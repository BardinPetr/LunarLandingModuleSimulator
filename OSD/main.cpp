#define IP 127.0.0.1
#define ID 0

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

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
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <iostream>
#include <windows.h>
#include <thread>
#include <time.h>

using namespace std;
using namespace cv;

const char* cw1 = "First camera";
const char* cw2 = "Second camera";
const char* cw3 = "xxxxxxxxxxxxx";
string url1 = "http://127.0.0.1:8888/out.jpg";
string url2 = "http://127.0.0.1:8889/out.jpg";
Size screensize = Size(1366, 768); //1366, 768
Point screen1 = Point(0, 0);
Point screen2 = Point(0, 0);
VideoCapture vcap1;
VideoCapture vcap2;
Mat _img1, _img2;
Mat img1, img2;
Mat img, _img;

double lasttime = 0.0;
double fps = 0.0;
bool f = true;
int c = 0;

double millis() {
    SYSTEMTIME st;
    GetSystemTime(&st);
    return st.wMilliseconds / 1000.0;
}

Mat getCam(int id) {
    (id == 0 ? vcap1 : vcap2).open((id == 0 ? url1 : url2));
    Mat a;
    if (!(id == 0 ? vcap1 : vcap2).isOpened()){
        IplImage* img = cvLoadImage("D:/i1.png", 1);
        a = cvarrToMat(img);
    }
    else{
        (id == 0 ? vcap1 : vcap2).read(a);
    }
    (id == 0 ? vcap1 : vcap2).release();
    resize(a, a, screensize);
    return a;
}

void update1() {
    putText(img1, to_string(fps), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
    //if (c % 50 == 0) {
    //    fps = (double(c) / (millis() - lasttime));
    //    lasttime = millis();
    //    c = 0;
    //}
    //c++;
}

void update2() {
    putText(img2, to_string(fps), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
    if (c % 50 == 0) {
        fps = (double(c) / (millis() - lasttime));
        lasttime = millis();
        c = 0;
    }
    c++;
}

void run1() {
    //lasttime = millis();

    img1 = getCam(1);
    while (f) {
        thread thr1(update1);
        _img1 = getCam(1);
        thr1.join();
        try {
            //imshow(cw1, img1);
        }
        catch (Exception) {}
        img1 = _img1;

        if (waitKey(1) == 27) break;
    }
    f = false;
}

void prepareWindow(int id){
    cw3 = (id == 0 ? cw1 : cw2);
    screen3 = (id == 0 ? screen1 : screen2);
    namedWindow(cw3, WND_PROP_FULLSCREEN);
    setWindowProperty(cw3, WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    cvMoveWindow(cw3, screen3.x, screen3.y);
}

int main()
{
    prepareWindows();

    lasttime = millis();

    img2 = getCam(ID);
    while (f) {
        thread thr(update2);
        _img = getCam(ID);
        thr.join();
        try {
            imshow(cw2, img2);
        }
        catch (Exception) {}
        img2 = _img2;

        if (waitKey(1) == 27) break;
    }
    return 0;
}
