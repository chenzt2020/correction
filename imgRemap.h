#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Eigen/Dense"

//将图像src通过基准线集vLine，重映射为图像dst
void imgRemap(const cv::Mat src, cv::Mat& dst, const std::vector<std::vector<cv::Point>> vLine, const int n = 5) {
	int nPoint = 0;
	for (auto it = vLine.begin(); it < vLine.end(); it++)
		nPoint += it->size();

	int* x = (int*)malloc(nPoint * sizeof(int));
	int* y = (int*)malloc(nPoint * sizeof(int));
	double* z = (double*)malloc(nPoint * sizeof(double));
	if (((size_t)x & (size_t)y & (size_t)z) == NULL)return;

	int l = 0;
	for (auto it = vLine.begin(); it < vLine.end(); it++) { // 计算新的基准线坐标
		double avgY = 0;
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			avgY += jt->y;
		}
		avgY = avgY + (it->end() - 1)->y;
		avgY /= it->size();
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			x[l] = jt->x;
			y[l] = jt->y;
			z[l] = avgY;
			l++;
		}
		x[l] = (it->end() - 1)->x;
		y[l] = (it->end() - 1)->y;
		z[l] = avgY;
		l++;
	}
	printf("..p:%d\n", l);

	Eigen::MatrixXd A(nPoint, (n + 1) * (n + 2) / 2);
	Eigen::MatrixXd b(nPoint, 1);
	for (int i = 0; i < nPoint; i++) {
		l = 0;
		for (int j = 0; j <= n; j++) {
			for (int k = j; k >= 0; k--) {
				A(i, l) = 1.0 * pow(x[i], k) * pow(z[i], j - k);
				l++;
			}
		}
		b(i, 0) = y[i];
	}
	free(x);
	free(y);
	free(z);

	Eigen::VectorXd xw;
	if (n > 5)
		xw = A.householderQr().solve(b);
	else xw = A.colPivHouseholderQr().solve(b); // 最小二乘法拟合曲面
	std::cout << "  w:\n";
	std::cout << xw << std::endl;

	int height = src.rows;
	int width = src.cols;
	cv::Mat xMap(src.size(), CV_32FC1);
	cv::Mat yMap(src.size(), CV_32FC1);
	for (int yi = 0; yi < height; yi++) {
		for (int xj = 0; xj < width; xj++) {
			xMap.at<float>(yi, xj) = xj;
			l = 0;
			double yNew = 0;
			for (int i = 0; i <= n; i++) {
				for (int j = i; j >= 0; j--) {
					yNew += xw(l) * pow(xj, j) * pow(yi, i - j); // 通过拟合出的曲面得到新的y值
					l++;
				}
			}
			yMap.at<float>(yi, xj) = yNew;
		}
	}
	cv::remap(src, dst, xMap, yMap, cv::INTER_CUBIC);
}