#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include "myHead.h"
#define DMorph 50
#define mergeTimes 10
using namespace std;
using namespace cv;

int main() {
	Mat img;
	string pureName = imgRead("bp4.jpg", img);
	Mat imgGray;
	vector<Rect>vRect;
	vector<vector<Point>>vLine;
	
	pretreat(img, imgGray);
	cvtColor(imgGray, img, cv::COLOR_GRAY2BGR);
	//imwrite(pureName + "_a.jpg", imgGray);
	bfs(imgGray, vRect);
	cout << "[]r:" << vRect.size() << "\n";
	mergeRect(vRect, mergeTimes);
	cout << "[]r:" << vRect.size() << "\n";
	getLine(vRect, vLine);
	sortLine(vLine);
	cout << "--l:" << vLine.size() << "\n";

	drawRect(img, vRect);
	//imwrite(pureName + "_b" + to_string(mergeTimes) + ".jpg", img);
	lineWrite(vLine, pureName);

	Mat imgCopy = img.clone();
	drawLine(imgCopy, vLine);
	imwrite(pureName + "_c.jpg", imgCopy);

	/*Mat imgOut;
	imgRemap(img0, imgOut);
	imwrite(pureName + "_cv.jpg", imgOut);*/
	return 0;
}
/*
	1.映射 yes

	2.清除竖直基准线右边的小噪点 yes

	3.准确读取横基准线 no

*/