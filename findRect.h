#pragma once

/// <summary>
/// 从图像src中搜索颜色为_color的连通域，并以矩形形式存到容器vRect里
/// </summary>
/// <param name="src">待搜索图像</param>
/// <param name="vRect">待存矩形容器</param>
/// <param name="_color">待搜索颜色</param>
/// <returns>矩形个数</returns>
int bfs(const cv::Mat src, std::vector<cv::Rect>& vRect, const uchar _color = 0) {
	int height = src.rows;
	int width = src.cols;
	int nTotalPx = height * width;
	bool* mark = (bool*)calloc(nTotalPx, sizeof(bool));
	if (mark == NULL)return -1;

	std::queue<int>q;
	for (int i = 0; i < nTotalPx; i++) { // 遍历所有像素，将连通用矩形记录
		if (src.data[i] == _color && mark[i] == 0) {
			int xMax = 0, yMin = width - 1, yMax = 0;
			q.emplace(i);
			while (!q.empty()) {
				int s = q.front();
				q.pop();
				if (s < 0 || s >= nTotalPx)continue;
				if (mark[s] || src.data[s] != _color)continue;
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
			vRect.emplace_back(cv::Point(yMin, i / width), cv::Point(yMax + 1, xMax + 1)); // 将连通域记录为矩形（bbox）
		}
	}

	for (auto it = vRect.begin(); it < vRect.end();) { // 去除过长过宽的矩形
		if (it->width > 200 || it->height > 200) {
			it = vRect.erase(it);
		}
		else it++;
	}
	free(mark);
	mark = NULL;
	return 0;
}