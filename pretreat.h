#pragma once

/// <summary>
/// 通过形态学闭运算和OTSU预处理图像
/// </summary>
/// <param name="src">源图像</param>
/// <param name="dst">目标图像</param>
/// <param name="DMorph">形态学闭运算的核大小</param>
void pretreat(const cv::Mat src, cv::Mat& dst, const int DMorph = 50) {
	dst = src.clone();
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(DMorph, DMorph)); // 可变：核的种类，核的大小
	morphologyEx(dst, dst, cv::MORPH_BLACKHAT, kernel);
	cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
	threshold(dst, dst, 255, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
}