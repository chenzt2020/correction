#pragma once
#include "Eigen/Dense"

/// <summary>
/// 将图像src通过基准线集vLine，重映射为图像dst
/// </summary>
/// <param name="src">源图像</param>
/// <param name="dst">目标图像</param>
/// <param name="vLine">基准线集</param>
/// <param name="n">曲线拟合的次数（1&lt;=n&lt;=5）</param>
void imgRemap(const cv::Mat src, cv::Mat& dst, const std::vector<std::vector<cv::Point>> vLine, const int n = 4) {
	if (n <= 0 || n >= 6)return;
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

	// n=5时，运行时间约1ms，无需优化
	for (int i = 0; i < nPoint; i++) {
		l = 0;
		for (int j = 0; j <= n; j++) {
			for (int k = j; k >= 0; k--) {
				A(i, l) = (double)pow(x[i], k) * pow(z[i], j - k);
				l++;
			}
		}
		b(i, 0) = y[i];
	}
	free(x);
	free(y);
	free(z);

	Eigen::VectorXd xw;
	if (n > 4)
		xw = A.householderQr().solve(b);
	else xw = A.colPivHouseholderQr().solve(b); // 最小二乘法拟合曲面
	std::cout << "  w:\n";
	std::cout << xw << std::endl;

	int height = src.rows;
	int width = src.cols;
	cv::Mat xMap(src.size(), CV_32FC1);
	cv::Mat yMap(src.size(), CV_32FC1, cv::Scalar(0));

	clock_t start = clock();
	// 通过拟合出的曲面计算像素的新坐标，占用了程序70%-90%的时间，因此进行优化
	for (int yi = 0; yi < height; yi++) {
		for (int xj = 0; xj < width; xj++) {
			xMap.at<float>(yi, xj) = xj;

			// n=4时约运行3200ms，n=5时约运行5000ms
			/*l = 0;
			for (int i = 0; i <= n; i++) {
				for (int j = i; j >= 0; j--) {
					yMap.at<float>(yi, xj) += xw[l] * pow(xj, j) * pow(yi, i - j);
					l++;
				}
			}*/

			// 循环展开，减少开销，n=4时约运行800ms，性能提高3倍，n=5时约运行1000ms，性能提高4倍
			switch (n) {
			case 5:
				yMap.at<float>(yi, xj) += xw[15] * pow(xj, 5) + xw[16] * pow(xj, 4) * yi + xw[17] * pow(xj, 3) * pow(yi, 2)
					+ xw[18] * pow(xj, 2) * pow(yi, 3) + xw[19] * xj * pow(yi, 4) + xw[20] * pow(yi, 5);

			case 4:
				yMap.at<float>(yi, xj) += xw[10] * pow(xj, 4) + xw[11] * pow(xj, 3) * yi
					+ xw[12] * pow(xj, 2) * pow(yi, 2) + xw[13] * xj * pow(yi, 3) + xw[14] * pow(yi, 4);

			case 3:
				yMap.at<float>(yi, xj) += xw[6] * pow(xj, 3) + xw[7] * pow(xj, 2) * yi
					+ xw[8] * xj * pow(yi, 2) + xw[9] * pow(yi, 3);

			case 2:
				yMap.at<float>(yi, xj) += xw[3] * pow(xj, 2) + xw[4] * xj * yi + xw[5] * pow(yi, 2);

			case 1:
				yMap.at<float>(yi, xj) += xw[0] + xw[1] * xj + xw[2] * yi;
			}
		}
	}
	clock_t end = clock();
	std::cout << end - start << "ms" << std::endl;
	cv::remap(src, dst, xMap, yMap, cv::INTER_CUBIC);
}