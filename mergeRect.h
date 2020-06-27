#pragma once

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
		while (1) {
			auto xl = std::lower_bound(vSrc.begin(), vSrc.end(), cv::Rect(it->x - width, 0, 0, 0), cmp);
			auto xr = std::upper_bound(vSrc.begin(), vSrc.end(), cv::Rect(it->x + it->width + width, 0, 0, 0), cmp);
			//printf("\n-i:%d %d %d", i, xl - vSrc.begin(), xr - vSrc.begin());
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

/// <summary>
/// 将接近的矩形合并，多次合并会增加合并的搜索范围
/// </summary>
/// <param name="vRect">待合并矩形集</param>
/// <param name="_mergeTimes">合并的次数</param>
void mergeRect(std::vector<cv::Rect>& vRect, const int _mergeTimes) {
	for (int i = 0; i < _mergeTimes; i++) {
		std::vector<cv::Rect> preVRect(vRect);
		mergeRect(preVRect, vRect, i, i);
	}
}

// 将矩阵集vRect画到图像src上
void drawRect(cv::Mat& src, const std::vector<cv::Rect>vRect) {
	int i = 0;
	for (auto it : vRect) {
		rectangle(src, it, CV_RGB(255, 0, 0));
		putText(src, std::to_string(i++), it.tl() + cv::Point(0, 8), cv::FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0));
	}
}