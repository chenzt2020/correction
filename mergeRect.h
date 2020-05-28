#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

void mergeRect(std::vector<cv::Rect> vSrc, std::vector<cv::Rect>& vDst,
	const int width, const int height) {
	auto cmp = [](const cv::Rect& a, const cv::Rect& b) {return a.x < b.x; };
	sort(vSrc.begin(), vSrc.end(), cmp);
	vDst.clear();
	bool* mark = (bool*)calloc(vSrc.size(), sizeof(bool));
	if (mark == NULL)return;
	int i = 0;
	for (auto it = vSrc.begin(); it < vSrc.end(); it++, i++) {
		if (mark[i])continue;
		mark[i] = 1;
		//printf("\n-i:%d", i);
		while (1) {
			auto xl = std::lower_bound(vSrc.begin(), vSrc.end(), cv::Rect(it->x - width, 0, 0, 0), cmp);
			auto xr = std::upper_bound(vSrc.begin(), vSrc.end(), cv::Rect(it->x + it->width + width, 0, 0, 0), cmp);
			//printf("\n-i:%d %d %d", i, xl - v.begin(), xr - v.begin());
			bool flag = 0;
			cv::Rect rectExpand = *it + cv::Size(2 * width, 2 * height) - cv::Point(width, height);
			for (auto jt = xl; jt < xr; jt++) {
				int j = jt - vSrc.begin();
				if (mark[j])continue;
				int crossArea = (rectExpand & *jt).area();
				if ((rectExpand & *jt).area() > 0) {
					cv::Rect merge = *it | *jt;
					if (merge.height > 60 || merge.width > 80)continue;
					mark[j] = 1;
					flag = 1;
					*it = merge;
				}
			}
			if (!flag)break;
		}
		vDst.emplace_back(*it);
	}
	free(mark);
	mark = NULL;
}

/* @brief ��δ�ϲ��ľ��κϲ�����κϲ������Ӻϲ���������Χ
@param vRect ���ϲ����μ�
@param _mergeTimes �ϲ��Ĵ���
*/
void mergeRect(std::vector<cv::Rect>& vRect, const int _mergeTimes) {
	for (int i = 0; i < _mergeTimes; i++) {
		std::vector<cv::Rect> preVRect(vRect);
		mergeRect(preVRect, vRect, i, i);
	}
}

//������vRect����ͼ��src��
void drawRect(cv::Mat& src, const std::vector<cv::Rect>vRect) {
	int i = 0;
	for (auto it = vRect.begin(); it < vRect.end(); i++, it++) {
		rectangle(src, *it, CV_RGB(255, 0, 0));
		putText(src, std::to_string(i), it->tl(), cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 0));
	}
}