#include <opencv2/opencv.hpp>
#include <iostream>
#include "myHead.h"
#define DMorph 50
#define mergeTimes 10
using namespace std;
using namespace cv;

int main() {
	Mat img;
	string fileName;
	cout << "要处理的文件：";
	cin >> fileName;
	string pureName = imgRead(fileName, img);
	Mat img0 = img.clone();
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

	/*drawRect(img, vRect);
	imwrite(pureName + "_b" + to_string(mergeTimes) + ".jpg", img);
	lineWrite(vLine, pureName);
	Mat imgCopy = img.clone();
	drawLine(imgCopy, vLine);
	imwrite(pureName + "_c.jpg", imgCopy);*/

	Mat imgOut;
	int nFit;
	cout << "曲面拟合的阶数（推荐5阶以下）：";
	cin >> nFit;
	imgRemap(img0, imgOut, vLine, nFit);
	imwrite(pureName + "_cv.jpg", imgOut);

	system("pause");
	return 0;
}
/*
	1.映射 yes

	2.清除竖直基准线右边的小噪点 yes

	3.准确读取横基准线 no

*/