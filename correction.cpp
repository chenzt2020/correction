#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include "myHead.h"
#define N_FIT 4
using namespace std;
using namespace cv;

int main() {
	Mat img;
	string fileName = "1.jpg";
	//cout << "要处理的文件：";
	//cin >> fileName;
	string pureName = imgRead(fileName, img);
	Mat imgGray, img1;
	vector<Rect>vRect;
	vector<vector<Point>>vLine;

	pretreat(img, imgGray);
	cvtColor(imgGray, img1, cv::COLOR_GRAY2BGR);
	//imwrite(pureName + "_a.jpg", imgGray);

	bfs(imgGray, vRect);
	cout << "[]r:" << vRect.size() << "\n";
	mergeRect(vRect, 10);
	cout << "[]r:" << vRect.size() << "\n";
	getLine(vRect, vLine);
	sortLine(vLine);
	cout << "--l:" << vLine.size() << "\n";
	//drawRect(img1, vRect);
	//imwrite(pureName + "_b" + to_string(mergeTimes) + ".jpg", img1);
	//lineWrite(vLine, pureName);
	//Mat img2 = img1.clone();
	//drawLine(img2, vLine);
	//imwrite(pureName + "_c.jpg", img2);

	Mat imgOut;
	//cout << "曲面拟合的阶数（1~5阶）：";
	//cin >> nFit;
	imgRemap(img, imgOut, vLine, N_FIT);
	//imwrite(pureName + "_cv.jpg", imgOut);

	return 0;
}