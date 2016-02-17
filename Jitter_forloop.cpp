#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	Mat src = imread("data/snn1.jpg");
	cv::imshow("src",src);
	Mat dst = Mat(src.size(),CV_8UC3);

	int heigh = src.rows;
	int width = src.cols;
	int BrushSize = 5;

	int radius = 2 * BrushSize + 1;

	//unsigned long AAtime = 0, BBtime = 0; //check processing time
	//AAtime = getTickCount(); //check processing time
	for(int y = BrushSize; y < (heigh - BrushSize); y++)
	{
		for(int x = BrushSize; x < (width - BrushSize); x++)
		{
			int nx = (x - BrushSize) + (rand() % radius - 0.5);
			int ny = (y - BrushSize) + (rand() % radius - 0.5);

			double b = src.at<cv::Vec3b>(ny, nx)[0];
			double g = src.at<cv::Vec3b>(ny, nx)[1];
			double r = src.at<cv::Vec3b>(ny, nx)[2];

			dst.at<Vec3b>(y,x)[0] = b;
			dst.at<Vec3b>(y,x)[1] = g;
			dst.at<Vec3b>(y,x)[2] = r;
		}
	}
	//BBtime = getTickCount(); //check processing time 
	//printf("%.2lf sec \n",  (BBtime - AAtime) / getTickFrequency() ); //check processing time

	cv::imshow("dst",dst);

	cv::waitKey();
	return 0;
}