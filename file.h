#pragma once
#include <fstream>

// ��ȡͼ��dst��������ͼ�����ƣ�������չ����
std::string imgRead(std::string imgPath, cv::Mat& dst) {
	std::string pureName = imgPath.substr(0, imgPath.rfind("."));
	clock_t start = clock();
	dst = cv::imread(imgPath);
	printf("imread:%dms\n", clock() - start);
	if (!dst.data) {
		printf("��ȡͼ��ʧ��\n");
		exit(-1);
	}
	else return pureName;
}

// �����׼��(ÿ��ֱ�ߵ��������ֵ)��������׼�ߺͻ�׼�������.csv�ļ�
void lineWrite(const std::vector<std::vector<cv::Point>> vLine, const std::string imgName) {
	std::ofstream fout(imgName + ".csv");
	if (!fout) {
		std::cout << imgName << ".csv�޷���";
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