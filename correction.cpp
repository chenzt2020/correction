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
	string fileName = "1.jpg";
	string pureName = fileName.substr(0, fileName.rfind("."));
	Mat img = imread(fileName);
	Mat imgGray;
	int width = img.cols, height = img.rows;
	std::vector<cv::Rect>rectV;
	Rect xBaseline, yBaseline;
	std::vector<std::vector<cv::Point>>lineV;

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(DMorph, DMorph));
	morphologyEx(img, img, MORPH_BLACKHAT, kernel);
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	threshold(imgGray, imgGray, 255, 255, THRESH_OTSU | THRESH_BINARY_INV);
	cvtColor(imgGray, img, COLOR_GRAY2BGR);

	imwrite(pureName + "_a.jpg", imgGray);

	bfs(imgGray, rectV);

	for (auto it = rectV.begin(); it < rectV.end();) {
		if (it->width > width / 3) {
			xBaseline = *it;
			it = rectV.erase(it);
		}
		else if (it->height > height / 3) {
			yBaseline = *it;
			it = rectV.erase(it);
		}
		else it++;
	}

	printf("--r:%d\n", rectV.size());
	mergeRect(rectV, mergeTimes);
	printf("[]r:%d\n", rectV.size());
	getLine(rectV, lineV);
	sortLine(lineV);
	printf("--l:%d\n", lineV.size());

	drawRect(img, rectV);
	imwrite(pureName + "_b" + to_string(mergeTimes) + ".jpg", img);
	printLine(lineV, pureName);

	Mat imgCopy = img.clone();
	drawLine(imgCopy, lineV);
	imwrite(pureName + "_c.jpg", imgCopy);

	/*Mat imgOut;
	imgRemap(img0, imgOut);

	imwrite(pureName + "_out.jpg", imgOut);*/
	return 0;
}
/*
	1.映射

	2.清除竖直基准线右边的小噪点

	*3.准确读取横基准线

*/