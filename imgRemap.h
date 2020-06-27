#pragma once
#include "Eigen/Dense"

/// <summary>
/// ��ͼ��srcͨ����׼�߼�vLine����ӳ��Ϊͼ��dst
/// </summary>
/// <param name="src">Դͼ��</param>
/// <param name="dst">Ŀ��ͼ��</param>
/// <param name="vLine">��׼�߼�</param>
/// <param name="n">������ϵĴ�����1&lt;=n&lt;=5��</param>
void imgRemap(const cv::Mat src, cv::Mat& dst, const std::vector<std::vector<cv::Point>> vLine, const int n = 4) {
	if (n <= 0 || n >= 6)return;
	int nPoint = 0;
	for (auto it : vLine)
		nPoint += it.size();

	int* x = (int*)malloc(nPoint * sizeof(int));
	int* y = (int*)malloc(nPoint * sizeof(int));
	double* z = (double*)malloc(nPoint * sizeof(double));
	if (((size_t)x & (size_t)y & (size_t)z) == 0)return;

	int l = 0;
	for (auto it : vLine) {
		double avgY = 0;
		for (auto jt : it)
			avgY += jt.y;
		avgY /= it.size();
		for (auto jt : it) {
			x[l] = jt.x;
			y[l] = jt.y;
			z[l] = avgY;
			l++;
		}
	}
	printf("..p:%d\n", l);

	Eigen::MatrixXd A(nPoint, (n + 1) * (n + 2) / 2);
	Eigen::MatrixXd b(nPoint, 1);
	// n=5ʱ������ʱ��Լ1ms�������Ż�
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
	else xw = A.colPivHouseholderQr().solve(b); // ��С���˷��������
	std::cout << "  w:\n" << xw << "\n";

	cv::Mat xMap(src.size(), CV_32FC1);
	cv::Mat yMap(src.size(), CV_32FC1);
	clock_t start = clock();
	// ��������+ѭ��չ���������ٿ�����n=4ʱԼ����400ms���������7����n=5ʱԼ����500ms���������9��
	xMap.forEach<float>(
		[](float& val, const int* pos) {
			val = pos[1];
		});
	switch (n) {
	case 5:
		yMap.forEach<float>(
			[xw](float& val, const int* pos) {
				val = xw[15] * pow(pos[1], 5) + xw[16] * pow(pos[1], 4) * pos[0] + xw[17] * pow(pos[1], 3) * pow(pos[0], 2)
					+ xw[18] * pow(pos[1], 2) * pow(pos[0], 3) + xw[19] * pos[1] * pow(pos[0], 4) + xw[20] * pow(pos[0], 5)
					+ xw[10] * pow(pos[1], 4) + xw[11] * pow(pos[1], 3) * pos[0]
					+ xw[12] * pow(pos[1], 2) * pow(pos[0], 2) + xw[13] * pos[1] * pow(pos[0], 3) + xw[14] * pow(pos[0], 4)
					+ xw[6] * pow(pos[1], 3) + xw[7] * pow(pos[1], 2) * pos[0]
					+ xw[8] * pos[1] * pow(pos[0], 2) + xw[9] * pow(pos[0], 3)
					+ xw[3] * pow(pos[1], 2) + xw[4] * pos[1] * pos[0] + xw[5] * pow(pos[0], 2)
					+ xw[0] + xw[1] * pos[1] + xw[2] * pos[0];
			});
		break;
	case 4:
		yMap.forEach<float>(
			[xw](float& val, const int* pos) {
				val = xw[10] * pow(pos[1], 4) + xw[11] * pow(pos[1], 3) * pos[0]
					+ xw[12] * pow(pos[1], 2) * pow(pos[0], 2) + xw[13] * pos[1] * pow(pos[0], 3) + xw[14] * pow(pos[0], 4)
					+ xw[6] * pow(pos[1], 3) + xw[7] * pow(pos[1], 2) * pos[0]
					+ xw[8] * pos[1] * pow(pos[0], 2) + xw[9] * pow(pos[0], 3)
					+ xw[3] * pow(pos[1], 2) + xw[4] * pos[1] * pos[0] + xw[5] * pow(pos[0], 2)
					+ xw[0] + xw[1] * pos[1] + xw[2] * pos[0];
			});
		break;
	case 3:
		yMap.forEach<float>(
			[xw](float& val, const int* pos) {
				val = xw[6] * pow(pos[1], 3) + xw[7] * pow(pos[1], 2) * pos[0]
					+ xw[8] * pos[1] * pow(pos[0], 2) + xw[9] * pow(pos[0], 3)
					+ xw[3] * pow(pos[1], 2) + xw[4] * pos[1] * pos[0] + xw[5] * pow(pos[0], 2)
					+ xw[0] + xw[1] * pos[1] + xw[2] * pos[0];
			});
		break;
	case 2:
		yMap.forEach<float>(
			[xw](float& val, const int* pos) {
				val = xw[3] * pow(pos[1], 2) + xw[4] * pos[1] * pos[0] + xw[5] * pow(pos[0], 2)
					+ xw[0] + xw[1] * pos[1] + xw[2] * pos[0];
			});
		break;
	case 1:
		yMap.forEach<float>(
			[xw](float& val, const int* pos) {
				val = xw[0] + xw[1] * pos[1] + xw[2] * pos[0];
			});
	}
	// ͨ����ϳ�������������ص������꣬ռ���˳���70%-90%��ʱ�䣬��˽����Ż�
	/*
	int height = src.rows;
	int width = src.cols;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			xMap.at<float>(row, col) = col;
			// n=4ʱԼ����3200ms��n=5ʱԼ����5000ms
			l = 0;
			for (int i = 0; i <= n; i++) {
				for (int j = i; j >= 0; j--) {
					yMap.at<float>(row, col) += xw[l] * pow(col, j) * pow(row, i - j);
					l++;
				}
			}
		}
	}*/
	std::cout << "remap-1:" << clock() - start << "ms\n";
	cv::remap(src, dst, xMap, yMap, cv::INTER_CUBIC); // �ɱ䣺��ֵ������
}