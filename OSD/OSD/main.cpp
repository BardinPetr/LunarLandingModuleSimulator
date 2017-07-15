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

Size screensize = Size(1300, 700); //1366, 768

double lasttime = 0.0;
double fps = 0.0;
bool f = true;
int c = 0;

double millis() {
	SYSTEMTIME st;
	GetSystemTime(&st);
	return st.wMilliseconds / 1000.0;
}

Mat getCam(bool id) {
	VideoCapture vcap;
	Mat a;
	string b = (id ? "8" : "9");
	vcap.open("http://127.0.0.1:888" + b + "/out.jpg");
	//vcap.open("D:\test.avi");
	vcap.read(a);
	//return (a.rows != 0 ? a : getCam(id));
	return a;
}

void update1(Mat &a) {
	putText(a, to_string(fps), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
	if (c % 10 == 0) {
		fps = (10.0 / (millis() - lasttime));
		lasttime = millis();
		c = 0;
	}
	c++;
}

void update2(Mat &a) {
	putText(a, to_string(fps), Point2f(10, 30), FONT_HERSHEY_SCRIPT_SIMPLEX, double(1), Scalar::all(255), 3, 8);
	if (c % 10 == 0) {
		fps = (10.0 / (millis() - lasttime));
		lasttime = millis();
		c = 0;
	}
	c++;
}

void run1() {
	Mat img1, _img1;
	lasttime = millis();

	img1 = getCam(false);
	while (f) {
		thread thr1(update1, img1);
		_img1 = getCam(false);
		thr1.join();
		try {
			imshow("test1", img1);
		}
		catch (Exception) {

		}
		img1 = _img1;

		if (waitKey(1) > 1) break;
	}
	f = false;
}

void run2() {
	Mat img2, _img2;
	lasttime = millis();

	img2 = getCam(true);
	while (f) {
		thread thr2(update2, img2);
		_img2 = getCam(true);
		thr2.join();
		try {
			imshow("test2", img2);
		}
		catch (Exception) {

		}
		img2 = _img2;

		if (waitKey(1) > 1) break;
	}
	f = false;
}

int main()
{
	//Mat x = getCam(false);
	//imshow("img", x);
	//cvWaitKey(0);
	//Mat y = cvCreateMat(screensize.height, screensize.width, DEPTH_MASK_16U);
	//cvResize(x.data, &y);
	//imshow("img", y);
	thread r1(run1);
	thread r2(run2);
	r1.join();
	r2.join();

	//Mat img1, _img1;
	//Mat img2, _img2;
	//lasttime = millis();

	//img1 = getCam(false);
	//img2 = getCam(true);
	//while (1) {
	//	thread thr1(update1, img1);
	//	thread thr2(update2, img2);
	//	_img1 = getCam(false);
	//	_img2 = getCam(true);
	//	thr1.join();
	//	thr2.join();
	//	imshow("test", img1);
	//	imshow("test2", img2);
	//	img1 = _img1;
	//	img2 = _img2;

	//	if (waitKey(1) > 1) break;
	//	cout << fps << endl;
	//}
	destroyAllWindows();
	return 0;
}