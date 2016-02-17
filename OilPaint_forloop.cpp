#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(){
	Mat src = imread("data/11.png",1);
	imshow("src",src);
	Mat dst = Mat(src.size(),CV_8UC3);

	int heigh = src.rows;
	int width = src.cols;
	int BrushSize = 5;
	int levels = 20;

	int* CountIntensity = new int[levels];
	int* RedAverage  = new int[levels];
	int* GreenAverage  = new int[levels];
	int* BlueAverage  = new int[levels];

	unsigned long AAtime = 0, BBtime = 0; //check processing time
	AAtime = getTickCount(); //check processing time
	for(int y = BrushSize; y < (heigh - BrushSize); y++)
	{
		for(int x = BrushSize; x < (width - BrushSize); x++)
		{
			//cout << "y " << y << " x " << x << endl;
			// initialize the array
			for(int i = 0; i < levels; i++)
			{
				CountIntensity[i] = 0;
				RedAverage[i] = 0;
				GreenAverage[i] = 0;
				BlueAverage[i] = 0;
			}

			for(int j = -BrushSize; j <= BrushSize; j++)
			{
				for(int i = -BrushSize; i <= BrushSize; i++)
				{
					//cout << "y+j " << y+j << " x+i " << x+i << endl;
					double b = src.at<cv::Vec3b>(y+j, x+i)[0];
					double g = src.at<cv::Vec3b>(y+j, x+i)[1];
					double r = src.at<cv::Vec3b>(y+j, x+i)[2];
					//cout << "y+j " << y+j << " x+i " << x+i << " " << b << " " << g << " " << r << endl;

					int nCurIntensity =  ( ( ( r + g + b ) / 3.0 ) * levels ) / 255;
					//cout << "y+j " << y+j << " x+i " << x+i << " " << nCurIntensity << endl;

                    int tmp = nCurIntensity;
                    CountIntensity[tmp]++;

                    BlueAverage[tmp] = BlueAverage[tmp] + b;
                    GreenAverage[tmp] = GreenAverage[tmp] + g;
                    RedAverage[tmp] = RedAverage[tmp] + r;
					//cout << "Average " << RedAverage[tmp] << endl;
				}
			}

			int nCurMax = 0;
            int nMaxIndex = 0;
            for( int nI = 0; nI < levels; nI++ )
            {
                if( CountIntensity[nI] > nCurMax )
                {
                    nCurMax = CountIntensity[nI];
                    nMaxIndex = nI;
                }
            }
			//cout << "CurMax " << nCurMax << " MaxIndex " << nMaxIndex << endl;

			double tmpB = BlueAverage[nMaxIndex] / nCurMax;
			double tmpG = GreenAverage[nMaxIndex] / nCurMax;
			double tmpR = RedAverage[nMaxIndex] / nCurMax;
			Vec3b pixelColor(tmpB,tmpG,tmpR);
			dst.at<Vec3b>(y,x) = pixelColor;
			//dst.at<Vec3b>(y,x)[0] = BlueAverage[nMaxIndex] / nCurMax;
			//dst.at<Vec3b>(y,x)[1] = GreenAverage[nMaxIndex] / nCurMax;
			//dst.at<Vec3b>(y,x)[2] = RedAverage[nMaxIndex] / nCurMax;
		}
	}
	BBtime = getTickCount(); //check processing time 
	printf("%.2lf sec \n",  (BBtime - AAtime) / getTickFrequency() ); //check processing time

	//for(int x = 0; x < levels; x++)
	//{
	//	cout << CountIntensity[x] << endl;
	//}
	imshow("dst",dst);

	cv::waitKey();
	return 0;
}