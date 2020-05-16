#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>

int bfsSub(
	const cv::Mat src,
	std::vector<cv::Rect>& v,
	bool* mark,
	const int start,
	const uchar _color
);
int bfs(
	const cv::Mat src,
	std::vector<cv::Rect>& v,
	const uchar _color = 0
);

int bfsSub(const cv::Mat src, std::vector<cv::Rect>& v, bool* mark, const int start, const uchar _color) {
	int height = src.rows;
	int width = src.cols;
	int nTotalPx = height * width;
	int nPixel = 0;
	int xMax = 0, yMin = width - 1, yMax = 0;
	std::queue<int>q;
	q.emplace(start);
	while (!q.empty()) {
		int s = q.front();
		q.pop();
		if (s < 0 || s >= nTotalPx || mark[s] || src.data[s] != _color)continue;
		nPixel++;
		mark[s] = 1;
		int x = s / width;
		int y = s % width;
		if (x > xMax)xMax = x;
		if (y < yMin)yMin = y;
		if (y > yMax)yMax = y;

		if (y < width - 1)q.emplace(s + 1); // 四邻域bfs
		if (y > 0)q.emplace(s - 1);
		q.emplace(s + width);
		q.emplace(s - width);
	}
	v.emplace_back(cv::Point(yMin, start / width), cv::Point(yMax + 1, xMax + 1)); // 将连通域记录为矩形（bbox）
	return nPixel;
}
int bfs(const cv::Mat src, std::vector<cv::Rect>& v, const uchar _color) {
	int height = src.rows;
	int width = src.cols;
	int nTotalPx = height * width;
	bool* mark = (bool*)calloc(nTotalPx, sizeof(bool));
	if (mark == NULL)return -1;
	int nRect = 0;
	for (int i = 0; i < nTotalPx; i++) { // 遍历所有像素，将连通用矩形记录
		if (src.data[i] == _color && mark[i] == 0) {
			if (bfsSub(src, v, mark, i, _color) > 0)
				nRect++;
		}
	}
	for (auto it = v.begin(); it < v.end();) { // 去除过长过宽的矩形
		if (it->width > width / 3 || it->height > height / 3) {
			it = v.erase(it);
		}
		else it++;
	}
	free(mark);
	mark = NULL;
	return nRect;
}