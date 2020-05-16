#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include "findRect.h"
#include "mergeRect.h"
#include "getLine.h"
#include "imgRemap.h"
#define DMorph 50
#define mergeTimes 10
using namespace std;
using namespace cv;

int main() {
	string fileName = "4.jpg";
	string pureName = fileName.substr(0, fileName.rfind("."));
	Mat img = imread(fileName);
	Mat img0 = img.clone();
	if (!img.data) {
		cout << "读取" << fileName << "失败\n";
		return -1;
	}
	Mat imgGray;
	int width = img.cols, height = img.rows;
	vector<Rect>rectV;
	vector<vector<Point>>lineV;

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(DMorph, DMorph));
	morphologyEx(img, img, MORPH_BLACKHAT, kernel);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	threshold(imgGray, imgGray, 255, 255, THRESH_OTSU | THRESH_BINARY_INV);
	cvtColor(imgGray, img, COLOR_GRAY2BGR);

	//imwrite(pureName + "_a.jpg", imgGray);
	bfs(imgGray, rectV);
	cout << "--r:" << rectV.size() << "\n";
	mergeRect(rectV, mergeTimes);
	cout << "[]r:" << rectV.size() << "\n";
	getLine(rectV, lineV);
	sortLine(lineV);
	cout << "--l:" << lineV.size() << "\n";

	drawRect(img, rectV);
	imwrite(pureName + "_b" + to_string(mergeTimes) + ".jpg", img);
	line2File(lineV, pureName);

	Mat imgCopy = img.clone();
	drawLine(imgCopy, lineV);
	imwrite(pureName + "_c.jpg", imgCopy);

	Mat imgOut;
	imgRemap(img0, imgOut);
	imwrite(pureName + "_cv.jpg", imgOut);
	return 0;
}
/*
	1.映射

	2.清除竖直基准线右边的小噪点

	3.准确读取横基准线

*/