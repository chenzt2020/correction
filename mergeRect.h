#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

void mergeRect(
	std::vector<cv::Rect> vSrc,
	std::vector<cv::Rect>& vDst,
	const int _DX = 2,
	const int _DY = 2,
	const int _area = 666
);
void mergeRect(
	std::vector<cv::Rect>& vSrc,
	const int _mergeTimes
);

void mergeRect(std::vector<cv::Rect> vSrc, std::vector<cv::Rect>& vDst,
	const int _DX, const int _DY, const int _area) {
	auto cmp = [](const cv::Rect& a, const cv::Rect& b) {return a.x < b.x; };
	sort(vSrc.begin(), vSrc.end(), cmp);
	vDst.clear();
	bool* mark = (bool*)calloc(vSrc.size() + 8, sizeof(bool));
	if (mark == NULL)return;
	int i = 0;
	for (auto it = vSrc.begin(); it < vSrc.end(); it++, i++) {
		if (mark[i])continue;
		mark[i] = 1;
		//printf("\n-i:%d", i);
		while (1) {
			auto xl = std::lower_bound(vSrc.begin(), vSrc.end(), cv::Rect(it->x - _DX, 0, 0, 0), cmp);
			auto xr = std::upper_bound(vSrc.begin(), vSrc.end(), cv::Rect(it->x + it->width + _DX, 0, 0, 0), cmp);
			//printf("\n-i:%d %d %d", i, xl - v.begin(), xr - v.begin());
			bool flag = 0;
			cv::Rect rect_ex = *it + cv::Size(2 * _DX, 2 * _DY) - cv::Point(_DX, _DY);
			for (auto jt = xl; jt < xr; jt++) {
				int j = jt - vSrc.begin();
				if (mark[j])continue;
				int crossArea = (rect_ex & *jt).area();
				if (it->area() > _area && jt->area() > _area && crossArea < 500)continue;
				if (crossArea) {
					mark[j] = 1;
					//printf("+");
					flag = 1;
					*it = *it | *jt;
				}
			}
			if (!flag)break;
		}
		vDst.emplace_back(*it);
	}
	free(mark);
	mark = NULL;
}
void mergeRect(std::vector<cv::Rect>& vSrc, const int _mergeTimes) {
	for (int i = 0; i < _mergeTimes; i++) {
		std::vector<cv::Rect> preVRect(vSrc);
		mergeRect(preVRect, vSrc, i, i);
		//printf("\n%d:%d", i + 1, rectV.size());
	}
}
void drawRect(cv::Mat& src, const std::vector<cv::Rect>_rectV) {
	int i = 0;
	for (auto it = _rectV.begin(); it < _rectV.end(); i++, it++) {
		rectangle(src, *it, CV_RGB(255, 0, 0));
		putText(src, std::to_string(i), it->tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 0));
	}
}