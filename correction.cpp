#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include "myHead.h"
#define N_FIT 4 // ÄâºÏ½×Êý
using namespace std;

int main(int argc, char* argv[]) {
	clock_t start = clock();
	if (argc == 1) {
		string imgPath;
		cout << "Image path£º";
		cin >> imgPath;
		start = clock();
		correct(imgPath);
	}
	if (argc == 2)correct(argv[1]);
	cout << "main:" << clock() - start << "ms\n";
	return 0;
}