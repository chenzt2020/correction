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
	cv::Mat yMap(src.size(), CV_32FC1);

	clock_t start = clock();
	// 循环展开，减少开销，n=4时约运行800ms，性能提高3倍，n=5时约运行1000ms，性能提高4倍
	switch (n) {
	case 5:
		for (int row = 0; row < height; row++)
			for (int col = 0; col < width; col++) {
				xMap.at<float>(row, col) = col;
				yMap.at<float>(row, col) = xw[15] * pow(col, 5) + xw[16] * pow(col, 4) * row + xw[17] * pow(col, 3) * pow(row, 2)
					+ xw[18] * pow(col, 2) * pow(row, 3) + xw[19] * col * pow(row, 4) + xw[20] * pow(row, 5)
					+ xw[10] * pow(col, 4) + xw[11] * pow(col, 3) * row
					+ xw[12] * pow(col, 2) * pow(row, 2) + xw[13] * col * pow(row, 3) + xw[14] * pow(row, 4)
					+ xw[6] * pow(col, 3) + xw[7] * pow(col, 2) * row
					+ xw[8] * col * pow(row, 2) + xw[9] * pow(row, 3)
					+ xw[3] * pow(col, 2) + xw[4] * col * row + xw[5] * pow(row, 2)
					+ xw[0] + xw[1] * col + xw[2] * row;
			}
		break;
	case 4:
		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				xMap.at<float>(row, col) = col;
				yMap.at<float>(row, col) = xw[10] * pow(col, 4) + xw[11] * pow(col, 3) * row
					+ xw[12] * pow(col, 2) * pow(row, 2) + xw[13] * col * pow(row, 3) + xw[14] * pow(row, 4)
					+ xw[6] * pow(col, 3) + xw[7] * pow(col, 2) * row
					+ xw[8] * col * pow(row, 2) + xw[9] * pow(row, 3)
					+ xw[3] * pow(col, 2) + xw[4] * col * row + xw[5] * pow(row, 2)
					+ xw[0] + xw[1] * col + xw[2] * row;
			}
		}
		break;
	case 3:
		for (int row = 0; row < height; row++)
			for (int col = 0; col < width; col++) {
				xMap.at<float>(row, col) = col;
				yMap.at<float>(row, col) = xw[6] * pow(col, 3) + xw[7] * pow(col, 2) * row
					+ xw[8] * col * pow(row, 2) + xw[9] * pow(row, 3)
					+ xw[3] * pow(col, 2) + xw[4] * col * row + xw[5] * pow(row, 2)
					+ xw[0] + xw[1] * col + xw[2] * row;
			}
		break;
	case 2:
		for (int row = 0; row < height; row++)
			for (int col = 0; col < width; col++) {
				xMap.at<float>(row, col) = col;
				yMap.at<float>(row, col) = xw[3] * pow(col, 2) + xw[4] * col * row + xw[5] * pow(row, 2)
					+ xw[0] + xw[1] * col + xw[2] * row;
			}
		break;
	case 1:
		for (int row = 0; row < height; row++)
			for (int col = 0; col < width; col++) {
				xMap.at<float>(row, col) = col;
				yMap.at<float>(row, col) = xw[0] + xw[1] * col + xw[2] * row;
			}
	}
	// 通过拟合出的曲面计算像素的新坐标，占用了程序70%-90%的时间，因此进行优化
	/*for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			xMap.at<float>(row, col) = col;
			// n=4时约运行3200ms，n=5时约运行5000ms
			l = 0;
			for (int i = 0; i <= n; i++) {
				for (int j = i; j >= 0; j--) {
					yMap.at<float>(row, col) += xw[l] * pow(col, j) * pow(row, i - j);
					l++;
				}
			}
		}
	}*/
	printf("remap:%dms\n", clock() - start);
	cv::remap(src, dst, xMap, yMap, cv::INTER_CUBIC); // 可变：插值的种类
}