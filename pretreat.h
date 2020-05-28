#pragma once
#include <opencv2/opencv.hpp>
//将图像src预处理为图像dst，参数为形态学运算核大小
void pretreat(const cv::Mat src, cv::Mat& dst, const int DMorph = 50) {
	dst = src.clone();
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DMorph, DMorph));
	morphologyEx(dst, dst, cv::MORPH_BLACKHAT, kernel);
	cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
	threshold(dst, dst, 255, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
}