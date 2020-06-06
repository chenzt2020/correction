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
	for (auto it = vLine.begin(); it < vLine.end(); it++)
		nPoint += it->size();

	int* x = (int*)malloc(nPoint * sizeof(int));
	int* y = (int*)malloc(nPoint * sizeof(int));
	double* z = (double*)malloc(nPoint * sizeof(double));
	if (((size_t)x & (size_t)y & (size_t)z) == NULL)return;

	int l = 0;
	for (auto it = vLine.begin(); it < vLine.end(); it++) { // �����µĻ�׼������
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
	std::cout << "  w:\n";
	std::cout << xw << std::endl;

	int height = src.rows;
	int width = src.cols;
	cv::Mat xMap(src.size(), CV_32FC1);
	cv::Mat yMap(src.size(), CV_32FC1, cv::Scalar(0));

	clock_t start = clock();
	// ͨ����ϳ�������������ص������꣬ռ���˳���70%-90%��ʱ�䣬��˽����Ż�
	for (int yi = 0; yi < height; yi++) {
		for (int xj = 0; xj < width; xj++) {
			xMap.at<float>(yi, xj) = xj;

			// n=4ʱԼ����3200ms��n=5ʱԼ����5000ms
			/*l = 0;
			for (int i = 0; i <= n; i++) {
				for (int j = i; j >= 0; j--) {
					yMap.at<float>(yi, xj) += xw[l] * pow(xj, j) * pow(yi, i - j);
					l++;
				}
			}*/

			// ѭ��չ�������ٿ�����n=4ʱԼ����800ms���������3����n=5ʱԼ����1000ms���������4��
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