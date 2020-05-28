#pragma once
#include <opencv2/opencv.hpp>
//½«Í¼ÏñsrcÖØÓ³ÉäÎªÍ¼Ïñdst
void imgRemap(const cv::Mat src, cv::Mat& dst) {
	int height = src.rows;
	int width = src.cols;
	int nTotalPx = height * width;
	cv::Mat xMap, yMap;
	xMap.create(src.size(), CV_32FC1);
	yMap.create(src.size(), CV_32FC1);

	double
		p00 = 166.1,
		p10 = -0.7753,
		p01 = 0.9107,
		p20 = 0.00103,
		p11 = 0.0005531,
		p02 = -5.086e-05,
		p30 = -5.232e-07,
		p21 = -6.841e-07,
		p12 = -9.004e-08,
		p03 = 7.072e-08,
		p40 = 6.273e-11,
		p31 = 3.652e-10,
		p22 = 3.368e-11,
		p13 = 2.161e-11,
		p04 = -3.184e-11,
		p50 = 1.67e-14,
		p41 = -7.69e-14,
		p32 = 1.743e-15,
		p23 = -7.703e-15,
		p14 = -9.393e-16,
		p05 = 4.811e-15;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			xMap.at<float>(y, x) = x;
			yMap.at<float>(y, x) = p00 + p10 * x + p01 * y + p20 * x * x + p11 * x * y + p02 * y * y + p30 * x * x * x + p21 * x * x * y + p12 * x * y * y + p03 * y * y * y + p40 * x * x * x * x + p31 * x * x * x * y + p22 * x * x * y * y + p13 * x * y * y * y + p04 * y * y * y * y + p50 * x * x * x * x * x + p41 * x * x * x * x * y + p32 * x * x * x * y * y + p23 * x * x * y * y * y + p14 * x * y * y * y * y + p05 * y * y * y * y * y;
		}
	}

	cv::remap(src, dst, xMap, yMap, cv::INTER_CUBIC);
}