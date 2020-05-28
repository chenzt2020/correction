#pragma once
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <cmath>
#include "DisjSet.h"
#define PI 3.14159265358979323846

//从矩阵集vRect中搜索基准线，存到二维点集vLine中，合并宽度为width
void getLine(std::vector<cv::Rect>& vRect, std::vector<std::vector<cv::Point>>& vLine, const int width = 100) {
	for (auto it = vRect.begin(); it < vRect.end();) {
		if (it->area() < 200)it = vRect.erase(it); // 计算线段时忽略小矩形
		else it++;
	}
	std::vector<cv::Rect>vY(vRect);
	for (auto it = vY.begin(); it < vY.end(); it++) {
		it->x += it->width / 2;
		it->y += it->height / 2;
	}
	auto cmpY = [](const cv::Rect& a, const cv::Rect& b) {return a.y < b.y; };
	sort(vRect.begin(), vRect.end(), cmpY);
	sort(vY.begin(), vY.end(), cmpY);
	int n = vRect.size();
	DisjSet f(n);
	int i = 0;
	for (auto it = vY.begin(); it < vY.end(); i++, it++) {
		//printf("\ni:%d ", i);
		auto xl = std::lower_bound(vY.begin(), vY.end(), cv::Rect(0, it->y - it->height * 2 / 3, 0, 0), cmpY);
		auto xr = std::upper_bound(vY.begin(), vY.end(), cv::Rect(0, it->y + it->height * 2 / 3, 0, 0), cmpY);
		cv::Rect next;
		int nexti, dMin = 8192;
		for (auto jt = xl; jt < xr; jt++) {
			if (jt->x > it->x) {
				int d = jt->x - it->x + abs(jt->y - it->y); // 距离：暂取D4
				if (d < dMin) {
					dMin = d;
					next = *jt;
					nexti = jt - vY.begin();
					//printf("+");
				}
			}
		}
		if (next.x > it->x && next.x - next.width / 2 < it->x + it->width / 2 + width) {
			f.find(i); // 将连接的矩形用并查集记录
			f.to_union(i, nexti);
			//printf("next:%d", nexti);
		}
	}

	std::unordered_map<int, int>map; // 将并查集中的点连成直线
	int nLine = 0;
	for (int i = 0; i < n; i++) {
		if (f.find(i) == i) {
			map[i] = nLine;
			nLine++;
		}
	}
	vLine.clear();
	vLine.insert(vLine.begin(), nLine, std::vector<cv::Point>());
	for (int i = 0; i < n; i++) {
		vLine[map[f.find(i)]].emplace_back(vY[i].tl());
	}
}

//计算两个向量间角度（此处以点表示向量）
double vectorAngle(const cv::Point a, const cv::Point b) {
	double dot = a.x * b.x + a.y * b.y;
	double mold = sqrt(((double)a.x * a.x + a.y * a.y) * (b.x * b.x + b.y * b.y));
	return acos(dot / mold) * 180 / PI;
}

//进一步整理连线集，剔除过短的连线，剔除拐角过大的连线
void sortLine(std::vector<std::vector<cv::Point>>& vLine) {
	auto cmpX = [](const cv::Point& a, const cv::Point& b) {return a.x < b.x; };
	auto cmpY = [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b)
	{return a.begin()->y < b.begin()->y; };

	for (auto it = vLine.begin(); it < vLine.end();) {
		if (it->size() < 5) {// 少于5个点的连线删去
			it = vLine.erase(it);
			continue;
		}
		sort(it->begin(), it->end(), cmpX);
		if ((it->end() - 1)->x - it->begin()->x < 200)
			it = vLine.erase(it); // 过短的连线删去
		else it++;
	}
	for (auto it = vLine.begin(); it < vLine.end();) {
		bool eraseFlag = 0;
		for (auto jt = it->begin(); jt < it->end() - 2;) {
			double angle = vectorAngle(*jt - *(jt + 1), *(jt + 1) - *(jt + 2));
			if (angle > 45) { // 拐角超过45度的连线删去整条线
				eraseFlag = 1;
				break;
			}
			else if (angle > 30) { // 拐角超过30度的连线删去线上的一个点
				it->erase((jt + 1));
				jt = it->begin();
			}
			else jt++;
		}
		if (eraseFlag)it = vLine.erase(it);
		else it++;
	}
	sort(vLine.begin(), vLine.end(), cmpY);
}

//将连线集vLine画到图像src上
void drawLine(cv::Mat& src, const std::vector<std::vector<cv::Point>> vLine) {
	int i = 0;
	for (auto it = vLine.begin(); it < vLine.end(); i++, it++) {
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			line(src, *jt, *(jt + 1), CV_RGB(255 - 5 * i, 5 * i, 5 * i), 2);
		}
	}
}