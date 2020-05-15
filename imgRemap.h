#pragma once
#include <opencv2/opencv.hpp>
void imgRemap(const cv::Mat src, cv::Mat& dst) {
	int height = src.rows;
	int width = src.cols;
	int nTotalPx = height * width;
	cv::Mat xMap, yMap;
	xMap.create(src.size(), CV_32FC1);
	yMap.create(src.size(), CV_32FC1);

	double  p00 = 189.2,
		p10 = -0.1103,
		p01 = 0.5939,
		p20 = -0.0001594,
		p11 = 0.000545,
		p02 = 0.000303,
		p30 = -3.089e-08,
		p21 = -2.749e-08,
		p12 = -5.526e-07,
		p03 = -5.515e-08,
		p40 = 4.15e-11,
		p31 = 5.767e-11,
		p22 = 8.625e-11,
		p13 = 2.031e-10,
		p04 = -2.33e-11,
		p50 = 1.413e-14,
		p41 = -6.62e-14,
		p32 = 1.823e-14,
		p23 = -2.473e-14,
		p14 = -2.535e-14,
		p05 = 7.057e-15;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			xMap.at<float>(y, x) = x;
			yMap.at<float>(y, x) = p00 + p10 * x + p01 * y + p20 * x * x + p11 * x * y + p02 * y * y + p30 * x * x * x + p21 * x * x * y + p12 * x * y * y + p03 * y * y * y + p40 * x * x * x * x + p31 * x * x * x * y + p22 * x * x * y * y + p13 * x * y * y * y + p04 * y * y * y * y + p50 * x * x * x * x * x + p41 * x * x * x * x * y + p32 * x * x * x * y * y + p23 * x * x * y * y * y + p14 * x * y * y * y * y + p05 * y * y * y * y * y;
		}
	}

	remap(src, dst, xMap, yMap, cv::INTER_LINEAR);
}