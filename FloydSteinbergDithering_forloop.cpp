#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	Mat src = imread("data/SlamDunk.jpg");
	cv::imshow("src",src);

	int heigh = src.rows;
	int width = src.cols;
	double errorB = 0;
	double errorG = 0;
	double errorR = 0;

	for(int y = 0; y < (heigh - 1); y++)
	{
		for(int x = 0; x < (width - 1); x++)
		{
			double ob = src.at<Vec3b>(y,x)[0];
			double og = src.at<Vec3b>(y,x)[1];
			double or = src.at<Vec3b>(y,x)[2];
			//cout << "y " << y << " x " << x << " " << ob << " " << og << " " << or << endl;
			

			double nb = ob < 128 ? 0 : 255;
			double ng = og < 128 ? 0 : 255;
			double nr = or < 128 ? 0 : 255;
			//cout << "y " << y << " x " << x << " " << nb << " " << ng << " " << nr << endl;


			src.at<Vec3b>(y,x)[0] = nb;
			src.at<Vec3b>(y,x)[1] = ng;
			src.at<Vec3b>(y,x)[2] = nr;

			errorB = ob - nb;
			errorG = og - ng;
			errorR = or - nr;
			//cout << errorB << " " << errorG << " " << errorR << endl; 

			int tmp = -30;
			if(errorB < tmp)
				errorB = tmp;
			if(errorG < tmp)
				errorG = tmp;
			if(errorR < tmp)
				errorR = tmp;


			if(x < (width - 1))
			{
				src.at<Vec3b>(y, x+1)[0] += 7*errorB/16;
				src.at<Vec3b>(y, x+1)[1] += 7*errorG/16;
				src.at<Vec3b>(y, x+1)[2] += 7*errorR/16;
			}
			if(x > 1 && y < (heigh - 1))
			{
				src.at<Vec3b>(y+1, x-1)[0] += 3*errorB/16;
				src.at<Vec3b>(y+1, x-1)[1] += 3*errorG/16;
				src.at<Vec3b>(y+1, x-1)[2] += 3*errorR/16;
			}
			if(y < (heigh - 1))
			{
				src.at<Vec3b>(y+1, x)[0] += 5*errorB/16;
				src.at<Vec3b>(y+1, x)[1] += 5*errorG/16;
				src.at<Vec3b>(y+1, x)[2] += 5*errorR/16;
			}
			if(x < (width - 1) && y < (heigh - 1))
			{
				src.at<Vec3b>(y+1, x+1)[0] += errorB/16;
				src.at<Vec3b>(y+1, x+1)[1] += errorG/16;
				src.at<Vec3b>(y+1, x+1)[2] += errorR/16;
			}
		}
	}
	cv::imshow("dst",src);

	cv::waitKey();
	return 0;
}