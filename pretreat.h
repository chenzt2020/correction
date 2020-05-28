#pragma once
#include <opencv2/opencv.hpp>
//��ͼ��srcԤ����Ϊͼ��dst������Ϊ��̬ѧ����˴�С
void pretreat(const cv::Mat src, cv::Mat& dst, const int DMorph = 50) {
	dst = src.clone();
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(DMorph, DMorph));
	morphologyEx(dst, dst, cv::MORPH_BLACKHAT, kernel);
	cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
	threshold(dst, dst, 255, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
}