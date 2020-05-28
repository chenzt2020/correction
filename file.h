#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>

//��ȡͼ��dst��������ͼ�����ƣ�������չ����
std::string imgRead(std::string file, cv::Mat& dst) {
	std::string pureName = file.substr(0, file.rfind("."));
	dst = cv::imread(file);
	if (!dst.data) {
		printf("��ȡͼ��ʧ��\n");
		exit(-1);
	}
	else return pureName;
}

//�����׼��(ÿ��ֱ�ߵ��������ֵ)��������׼�ߺͻ�׼�������.csv�ļ�
void lineWrite(const std::vector<std::vector<cv::Point>> vLine, const std::string imgName) {
	std::ofstream fout(imgName + ".csv");
	if (!fout) {
		std::cout << imgName << ".csv�޷���";
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