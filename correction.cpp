#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include "myHead.h"
#define N_FIT 4 // 拟合阶数
using namespace std;
using namespace cv;

int main() {
	Mat img;
	string imgPath = "1.jpg";
	//cout << "Image path：";
	//cin >> imgPath;
	clock_t start = clock();
	string imgName = imgRead(imgPath, img);
	Mat imgGray, img1;
	vector<Rect>vRect;
	vector<vector<Point>>vLine;

	pretreat(img, imgGray);
	//cvtColor(imgGray, img1, cv::COLOR_GRAY2BGR);
	//imwrite(pureName + "_a.jpg", imgGray);

	bfs(imgGray, vRect);

	cout << "[]r:" << vRect.size() << "\n";
	mergeRect(vRect, 10);
	cout << "[]r:" << vRect.size() << "\n";
	getLine(vRect, vLine);
	sortLine(vLine);
	cout << "--l:" << vLine.size() << "\n";
	//drawRect(img1, vRect);
	//imwrite(pureName + "_b" + to_string(10) + ".jpg", img1); // 第二步文本提取结果
	//Mat img2 = img1.clone();
	//drawLine(img2, vLine);
	//imwrite(pureName + "_c.jpg", img2); // 第三步文本整理结果

	Mat imgOut;
	//int nFit;
	//cout << "曲面拟合的阶数（1~5阶）：";
	//cin >> nFit;
	imgRemap(img, imgOut, vLine, N_FIT);
	clock_t end = clock();
	imwrite(imgName + "_cv.jpg", imgOut);
	printf("imwrite:%dms\n", clock() - end);
	printf("main:%dms\nPress Enter to continue...", clock() - start);
	return 0;
}