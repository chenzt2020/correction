#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>

int bfs_sub(
	cv::Mat& src,
	std::vector<cv::Rect>& v,
	bool* mark,
	const int start,
	const bool _foreground
);
int bfs(
	const cv::Mat src,
	cv::Mat& dst,
	std::vector<cv::Rect>& v,
	const bool _foreground = 0
);

int bfs_sub(cv::Mat& src, std::vector<cv::Rect>& v, bool* mark, const int start, const bool _foreground) {
	int height = src.rows;
	int width = src.cols;
	int n_total = height * width;
	int n_px = 0;
	int x_min = start / width, x_max = 0, y_min = width - 1, y_max = 0;
	std::queue<int>q;
	std::vector<int>v_2;

	q.emplace(start);
	while (!q.empty()) {
		int s = q.front();
		q.pop();
		if (s < 0 || s >= n_total || mark[s] || src.data[s] != _foreground)continue;
		n_px++;
		mark[s] = 1;
		v_2.emplace_back(s);
		int x = s / width;
		int y = s % width;
		if (x > x_max)x_max = x;
		if (y < y_min)y_min = y;
		if (y > y_max)y_max = y;

		if (y < width - 1)q.emplace(s + 1);
		if (y > 0)q.emplace(s - 1);
		q.emplace(s + width);
		q.emplace(s - width);
	}

	if (n_px > 0) {
		v.emplace_back(cv::Point(y_min, x_min), cv::Point(y_max + 1, x_max + 1));
	}
	else {
		for (int i = 0; i < n_px; i++)src.data[v_2[i]] = 255;
	}
	return n_px;
}
int bfs(const cv::Mat src, cv::Mat& dst, std::vector<cv::Rect>& v, const bool _foreground) {
	int n_total_px = src.rows * src.cols;
	bool* mark = (bool*)calloc(n_total_px + 8, sizeof(bool));
	if (mark == NULL)return -1;
	dst = src.clone();
	int n_rect = 0;
	for (int i = 0; i < n_total_px; i++) {
		if (dst.data[i] == _foreground) {
			if (bfs_sub(dst, v, mark, i, _foreground) > 0)
				n_rect++;
		}
	}
	free(mark);
	mark = NULL;
	return n_rect;
}