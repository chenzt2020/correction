#pragma once
#include <fstream>

// 读取图像到dst，并返回图像名称（不带拓展名）
std::string imgRead(std::string imgPath, cv::Mat& dst) {
	std::string pureName = imgPath.substr(0, imgPath.rfind("."));
	clock_t start = clock();
	dst = cv::imread(imgPath);
	printf("imread:%dms\n", clock() - start);
	if (!dst.data) {
		printf("读取图像失败\n");
		exit(-1);
	}
	else return pureName;
}

// 计算基准点(每条直线的纵坐标均值)，并将基准线和基准点输出到.csv文件
void lineWrite(const std::vector<std::vector<cv::Point>> vLine, const std::string imgName) {
	std::ofstream fout(imgName + ".csv");
	if (!fout) {
		std::cout << imgName << ".csv无法打开";
		return;
	}
	std::string s;
	for (auto it : vLine) {
		double avgY = 0;
		for (auto jt : it)
			avgY += jt.y;
		avgY /= it.size();
		for (auto jt : it)
			s += cv::format("%d,%d,%lf\n", jt.x, jt.y, avgY);
	}
	fout << s;
	fout.close();
}