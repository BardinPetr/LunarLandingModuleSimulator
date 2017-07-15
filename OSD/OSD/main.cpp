#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

VideoCapture vcap;

Size screensize = Size(1300, 700); //1366, 768

double lasttime = 0.0;
double fps = 0.0;
int c = 0;

double millis() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return st.wMilliseconds / 1000.0;
}

Mat getCam(bool id) {
	Mat a;
	string b = (id ? "8" : "9");
	vcap.open("http://127.0.0.1:888" + b + "/out.jpg");
	vcap.read(a);
	return a;
}

void update(Mat &a) {
	//resize(a, a, screensize, 1, 1);
	putText(a, to_string(fps), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
	if (c % 10 == 0) {
		fps = (10.0 / (millis() - lasttime));
		lasttime = millis();
		c = 0;
	}
	c++;
}

int main()
{
	Mat img1, _img1;
	Mat img2, _img2;
	lasttime = millis();
	img1 = getCam(true);
	while (1) {
		thread thr(update, img1);
		_img1 = getCam(true);
		thr.join();
		imshow("test", img1);
		img1 = _img1;

		if (waitKey(1) > 1) break;
		cout << fps << endl;
	}
	destroyAllWindows();
	return 0;
}