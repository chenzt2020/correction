#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

//读取图像到dst，并返回图像名称（不带拓展名）
std::string imgRead(std::string file, cv::Mat& dst) {
	std::string pureName = file.substr(0, file.rfind("."));
	dst = cv::imread(file);
	if (!dst.data) {
		printf("读取图像失败\n");
		exit(-1);
	}
	else return pureName;
}

//计算基准点(每条直线的纵坐标均值)，并将基准线和基准点输出到.csv文件
void lineWrite(const std::vector<std::vector<cv::Point>> vLine, const std::string imgName) {
	std::ofstream fout(imgName + ".csv");
	if (!fout) {
		std::cout << imgName << ".csv无法打开";
		return;
	}
	std::string s;
	for (auto it = vLine.begin(); it < vLine.end(); it++) {
		double avgY = 0;
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			avgY += jt->y;
		}
		avgY = avgY + (it->end() - 1)->y;
		avgY /= it->size();
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			s += cv::format("%d,%d,%lf\n", jt->x, jt->y, avgY);
		}
		s += cv::format("%d,%d,%lf\n", (it->end() - 1)->x, (it->end() - 1)->y, avgY);
	}
	fout << s;
	fout.close();
}