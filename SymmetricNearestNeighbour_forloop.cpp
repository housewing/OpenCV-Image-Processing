#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	Mat src = imread("data/snn.png");
	imshow("src",src);
	Mat dst = Mat(src.size(),CV_8UC3);

	int heigh = src.rows;
	int width = src.cols;
	int BrushSize = 3;

	unsigned long AAtime = 0, BBtime = 0; //check processing time
	AAtime = getTickCount(); //check processing time
	for(int y = BrushSize; y < (heigh - BrushSize); y++)
	{
		for(int x = BrushSize; x < (width - BrushSize); x++)
		{
			double b_cen = src.at<cv::Vec3b>(y, x)[0];
			double g_cen = src.at<cv::Vec3b>(y, x)[1];
			double r_cen = src.at<cv::Vec3b>(y, x)[2];
			double sumB = 0;
			double sumG = 0;
			double sumR = 0;
			int count = 0;

			for(int j = -BrushSize; j <= 0; j++)
			{
				for(int i = -BrushSize; i <= BrushSize; i++)
				{
					double b_pre = src.at<cv::Vec3b>(y+j, x+i)[0];
					double g_pre = src.at<cv::Vec3b>(y+j, x+i)[1];
					double r_pre = src.at<cv::Vec3b>(y+j, x+i)[2];

					double b_next = src.at<cv::Vec3b>(y-j, x-i)[0];
					double g_next = src.at<cv::Vec3b>(y-j, x-i)[1];
					double r_next = src.at<cv::Vec3b>(y-j, x-i)[2];

					int error_pre = sqrt(abs(b_cen - b_pre) + abs(g_cen - g_pre) + abs(r_cen - r_pre));
					int error_next = sqrt(abs(b_cen - b_next) + abs(g_cen - g_next) + abs(r_cen - r_next));
					
					//cout << b_cen - b_pre << " " << g_cen - g_pre << " " << r_cen - r_pre << endl;
					//cout << error_pre << " " << error_next << endl;

					if(error_pre < error_next)
					{
						sumB += b_pre;
						sumG += g_pre;
						sumR += r_pre;
					}
					else
					{
						sumB += b_next;
						sumG += g_next;
						sumR += r_next;
					}
					count++;
				}
			}


			//cout << sumB << " " << sumG << " " << sumR << endl;

			//double tmpB = sumB / count;
			//double tmpG = sumG / count;
			//double tmpR = sumR / count;
			//Vec3b pixelColor(tmpB,tmpG,tmpR);
			//dst.at<Vec3b>(y,x) = pixelColor;

			dst.at<Vec3b>(y,x)[0] = sumB / count;
			dst.at<Vec3b>(y,x)[1] = sumG / count;
			dst.at<Vec3b>(y,x)[2] = sumR / count;
		}
	}
	BBtime = getTickCount(); //check processing time 
	printf("%.2lf sec \n",  (BBtime - AAtime) / getTickFrequency() ); //check processing time

	imshow("dst",dst);

	cv::waitKey();
	return 0;
}