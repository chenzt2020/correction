#pragma once

void correct(std::string imgPath, const int nFit = 4, const int type = 0) {
	cv::Mat img, imgGray, imgOut;
	std::string imgName = imgRead(imgPath, img);
	std::vector<cv::Rect>vRect;
	std::vector<std::vector<cv::Point>>vLine;
	pretreat(img, imgGray);
	switch (type) {
	case 0:
		bfs(imgGray, vRect);
		std::cout << "[]r:" << vRect.size() << "\n";
		mergeRect(vRect, 10);
		std::cout << "[]r:" << vRect.size() << "\n";
		getLine(vRect, vLine);
		sortLine(vLine);
		std::cout << "--l:" << vLine.size() << "\n";
		break;
	case 1:
		cv::Mat img1;
		cvtColor(imgGray, img1, cv::COLOR_GRAY2BGR);
		bfs(imgGray, vRect);
		std::cout << "[]r:" << vRect.size() << "\n";
		mergeRect(vRect, 10);
		std::cout << "[]r:" << vRect.size() << "\n";
		getLine(vRect, vLine);
		sortLine(vLine);
		std::cout << "--l:" << vLine.size() << "\n";
		drawRect(img1, vRect);
		imwrite(imgName + "_b10.jpg", img1); // 第二步文本提取结果
		drawLine(img1, vLine);
		imwrite(imgName + "_c.jpg", img1); // 第三步文本整理结果
	}
	imgRemap(img, imgOut, vLine, nFit);
	clock_t start = clock();
	imwrite(imgName + "_cv.jpg", imgOut);
	std::cout << "imwrite:" << clock() - start << "ms\n";
}