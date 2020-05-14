#include <opencv2/opencv.hpp>
#include <cstdio>
#include <iostream>
#include "findRect.h"
#include "mergeRect.h"
#include "getLine.h"
#define DKernal 25
#define mergeTimes 10
using namespace std;
using namespace cv;

int main() {
	string fileName = "1_sub.jpg";
	string pureName = fileName.substr(0, fileName.rfind("."));
	Mat img = imread(fileName);
	Mat img_g, img_gcopy;
	int width = img.cols, height = img.rows;
	std::vector<cv::Rect>rectV;
	Rect xBaseline, yBaseline;
	std::vector<std::vector<cv::Point>>lineV;

	int thresh = 50;
	cvtColor(img, img_g, COLOR_BGR2GRAY);
	threshold(img_g, img_g, 255, 255, THRESH_OTSU);

	bfs(img_g, img_gcopy, rectV);
	//imwrite(pureName + "_t0.jpg", img_gcopy);

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

	printf("\n~~r:%d", rectV.size());
	mergeRect(rectV, mergeTimes);
	printf("\n[]r:%d", rectV.size());
	getLine(rectV, lineV);
	sortLine(lineV);
	printf("\n--p:%d", lineV.size());

	int i = 0;
	for (auto it = rectV.begin(); it < rectV.end(); i++, it++) {
		rectangle(img, *it, CV_RGB(255, 0, 0));
		putText(img, to_string(i), it->tl(), FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 0));
	}
	imwrite(pureName + "_a" + to_string(mergeTimes) + ".jpg", img);
	printLine(lineV, pureName);

	Mat imgCopy = img.clone();
	i = 0;
	for (auto it = lineV.begin(); it < lineV.end(); i++, it++) {
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			line(imgCopy, *jt, *(jt + 1), CV_RGB(255 - 5 * i, 5 * i, 5 * i), 2);
		}
	}
	imwrite(pureName + "_b" + to_string(1) + ".jpg", imgCopy);

	return 0;
}