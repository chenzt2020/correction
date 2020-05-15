#pragma once
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "DisjSet.h"

void getLine(
	std::vector<cv::Rect>& vSrc,
	std::vector<std::vector<cv::Point>>& vDst,
	const int Dx = 100,
	const int Dy = 20
);
void sortLine(
	std::vector<std::vector<cv::Point>>& vSrc
);
void printLine(
	const std::vector<std::vector<cv::Point>> _lineV,
	const std::string _pureName
);

void getLine(std::vector<cv::Rect>& vSrc, std::vector<std::vector<cv::Point>>& vDst, const int Dx, const int Dy) {
	for (auto it = vSrc.begin(); it < vSrc.end();) {
		if (it->area() < 333)it = vSrc.erase(it); // 计算线段时忽略小矩形
		else it++;
	}
	std::vector<cv::Rect>vY(vSrc);
	for (auto it = vY.begin(); it < vY.end(); it++) {
		it->x += it->width / 2;
		it->y += it->height / 2;
	}
	auto cmpX = [](const cv::Rect& a, const cv::Rect& b) {return a.x < b.x; };
	auto cmpY = [](const cv::Rect& a, const cv::Rect& b) {return a.y < b.y; };
	sort(vSrc.begin(), vSrc.end(), cmpY);
	sort(vY.begin(), vY.end(), cmpY);
	int n = vSrc.size();
	DisjSet f(n);
	int i = 0;
	for (auto it = vY.begin(); it < vY.end(); i++, it++) {
		//printf("\n%d:%d %d %d", i, it->x, it->y, it->area());
		auto xl = std::lower_bound(vY.begin(), vY.end(), cv::Rect(0, it->y - it->height / 2 - Dy, 0, 0), cmpY);
		int xrInt;
		if (it->height % 2)xrInt = it->y + it->height / 2 + Dy + 1;
		else xrInt = it->y + it->height / 2 + Dy;
		auto xr = std::upper_bound(vY.begin(), vY.end(), cv::Rect(0, xrInt, 0, 0), cmpY);

		int next = i, xmin = 4096;
		for (auto jt = xl; jt < xr; jt++) {
			if (jt->x < xmin && jt->x > it->x) {
				xmin = jt->x;
				next = jt - vY.begin();
				//printf("+");
			}
		}
		if (xmin > it->x && xmin < it->x + it->width + Dx) {
			f.find(i);
			f.to_union(i, next);
			//printf("  next:%d", next);
		}
	}

	std::unordered_map<int, int>map;
	int nLine = 0;
	for (int i = 0; i < n; i++) {
		if (f.find(i) == i) {
			map[i] = nLine;
			nLine++;
		}
	}
	vDst.clear();
	vDst.insert(vDst.begin(), nLine, std::vector<cv::Point>());
	for (int i = 0; i < n; i++) {
		vDst[map[f.find(i)]].emplace_back(vY[i].tl());
	}
}
void sortLine(std::vector<std::vector<cv::Point>>& vSrc) {
	auto cmpX = [](const cv::Point& a, const cv::Point& b) {return a.x < b.x; };
	auto cmpY = [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b)
	{return a.begin()->y < b.begin()->y; };
	for (auto it = vSrc.begin(); it < vSrc.end();) {
		if (it->size() < 8)it = vSrc.erase(it);
		else {
			sort(it->begin(), it->end(), cmpX);
			it++;
		}
	}
	sort(vSrc.begin(), vSrc.end(), cmpY);
	/*printf("\n");
	for (auto it = vDst.begin(); it < vDst.end(); it++) {
		for (auto jt = it->begin(); jt < it->end(); jt++) {
			printf("\np:%d %d", jt->x, jt->y);
		}
		printf("\n");
	}*/
}
void printLine(const std::vector<std::vector<cv::Point>> _lineV, const std::string _pureName) {
	std::ofstream fout(_pureName + ".csv");
	if (!fout) {
		std::cout << _pureName << ".csv无法打开";
		return;
	}
	std::string s;
	for (auto it = _lineV.begin(); it < _lineV.end(); it++) {
		double avgY = 0;
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			avgY += jt->y;
		}
		avgY = avgY + (it->end() - 1)->y;
		avgY /= it->size();
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			s += cv::format("%d,%d,%lf\n", jt->x, jt->y, avgY);
		}
		s += cv::format("%d,%d,%lf\n", (it->end() - 1)->x, (it->end() - 1)->y, avgY);
	}
	fout << s;
	fout.close();
}
void drawLine(cv::Mat& src, const std::vector<std::vector<cv::Point>> _lineV) {
	int i = 0;
	for (auto it = _lineV.begin(); it < _lineV.end(); i++, it++) {
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			line(src, *jt, *(jt + 1), CV_RGB(255 - 5 * i, 5 * i, 5 * i), 2);
		}
	}
}