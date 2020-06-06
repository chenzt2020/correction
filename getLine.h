#pragma once
#include <unordered_map>
#include "DisjSet.h"
#define PI 3.14159265358979323846

/// <summary>
/// �Ӿ�����������׼��
/// </summary>
/// <param name="vRect">����������</param>
/// <param name="vLine">��׼�߼����Զ�ά�㼯��ʽ��ʾ</param>
/// <param name="width">�ϲ���ȣ�����������ڵ��������λᱻ���뵽һ������</param>
void getLine(std::vector<cv::Rect>& vRect, std::vector<std::vector<cv::Point>>& vLine, const int width = 100) {
	for (auto it = vRect.begin(); it < vRect.end();) {
		if (it->area() < 200)it = vRect.erase(it); // �����߶�ʱ����С����
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
				int d = jt->x - it->x + abs(jt->y - it->y); // ���룺��ȡD4
				if (d < dMin) {
					dMin = d;
					next = *jt;
					nexti = jt - vY.begin();
					//printf("+");
				}
			}
		}
		if (next.x > it->x && next.x - next.width / 2 < it->x + it->width / 2 + width) {
			f.find(i); // �����ӵľ����ò��鼯��¼
			f.to_union(i, nexti);
			//printf("next:%d", nexti);
		}
	}

	std::unordered_map<int, int>map; // �����鼯�еĵ�����ֱ��
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

//��������������Ƕȣ��˴��Ե��ʾ������
double vectorAngle(const cv::Point a, const cv::Point b) {
	double dot = a.x * b.x + a.y * b.y;
	double mold = sqrt(((double)a.x * a.x + a.y * a.y) * (b.x * b.x + b.y * b.y));
	return acos(dot / mold) * 180 / PI;
}

//��һ�������׼�߼����޳����̵����ߣ��޳��սǹ��������
void sortLine(std::vector<std::vector<cv::Point>>& vLine) {
	auto cmpX = [](const cv::Point& a, const cv::Point& b) {return a.x < b.x; };
	auto cmpY = [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b)
	{return a.begin()->y < b.begin()->y; };

	for (auto it = vLine.begin(); it < vLine.end();) {
		if (it->size() < 5) {// ����5���������ɾȥ
			it = vLine.erase(it);
			continue;
		}
		sort(it->begin(), it->end(), cmpX);
		if ((it->end() - 1)->x - it->begin()->x < 200)
			it = vLine.erase(it); // ���̵�����ɾȥ
		else it++;
	}
	for (auto it = vLine.begin(); it < vLine.end();) {
		bool eraseFlag = 0;
		for (auto jt = it->begin(); jt < it->end() - 2;) {
			double angle = vectorAngle(*jt - *(jt + 1), *(jt + 1) - *(jt + 2));
			if (angle > 45) { // �սǳ���45�ȵ�����ɾȥ������
				eraseFlag = 1;
				break;
			}
			else if (angle > 30) { // �սǳ���30�ȵ�����ɾȥ���ϵ�һ����
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

//�����߼�vLine����ͼ��src��
void drawLine(cv::Mat& src, const std::vector<std::vector<cv::Point>> vLine) {
	int i = 0;
	for (auto it = vLine.begin(); it < vLine.end(); i++, it++) {
		for (auto jt = it->begin(); jt < it->end() - 1; jt++) {
			line(src, *jt, *(jt + 1), CV_RGB(255 - 5 * i, 5 * i, 5 * i), 2);
		}
	}
}