#pragma once

/// <summary>
/// ͨ����̬ѧ�������OTSUԤ����ͼ��
/// </summary>
/// <param name="src">Դͼ��</param>
/// <param name="dst">Ŀ��ͼ��</param>
/// <param name="DMorph">��̬ѧ������ĺ˴�С</param>
void pretreat(const cv::Mat src, cv::Mat& dst, const int DMorph = 50) {
	dst = src.clone();
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(DMorph, DMorph)); // �ɱ䣺�˵����࣬�˵Ĵ�С
	morphologyEx(dst, dst, cv::MORPH_BLACKHAT, kernel);
	cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
	threshold(dst, dst, 255, 255, cv::THRESH_OTSU | cv::THRESH_BINARY_INV);
}