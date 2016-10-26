#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;

Mat canvas;

std::vector<Point> applyTransform(std::vector<Point> _points, cv::Mat _scale, cv::Point _reference, cv::Point2f _transPt, float _len)
{
	for (auto& p : _points)
	{
		Point tmp = p;
		tmp.x -= _reference.x;
		tmp.y -= _reference.y;
		p.x = tmp.x * _scale.at<float>(0, 0) + tmp.y * _scale.at<float>(0, 1) + _reference.x + _transPt.x * _len;
		p.y = tmp.x * _scale.at<float>(1, 0) + tmp.y * _scale.at<float>(1, 1) + _reference.y - _transPt.y * _len;
	}

	return _points;
}

Mat getScale(float _scale)
{
	cv::Mat transformation = cv::Mat::zeros(2, 2, CV_32FC1);
	transformation.at<float>(0, 0) = _scale;
	transformation.at<float>(0, 1) = 0.0;
	transformation.at<float>(1, 0) = 0.0;
	transformation.at<float>(1, 1) = _scale;

	return transformation;
}

void draw(std::vector<Point> _points, int _level)
{
	Mat scaleMatrix = getScale(1.0 / 3.0);
	Point2f lenPt;
	lenPt.x = _points[0].x - _points[1].x;
	lenPt.y = _points[0].y - _points[1].y;
	float length = std::sqrt(std::pow(lenPt.x, 2) + std::pow(lenPt.y, 2));

	std::vector<std::vector<Point>> ptSet(8);

	Point2f transPt = Point2f(0.0, 0.0);
	ptSet[0] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(0.0, 1.0 / 3.0);
	ptSet[1] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(0.0, 2.0 / 3.0);
	ptSet[2] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(1.0 / 3.0, 0.0);
	ptSet[3] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(1.0 / 3.0, 2.0 / 3.0);
	ptSet[4] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(2.0 / 3.0, 0.0);
	ptSet[5] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(2.0 / 3.0, 1.0 / 3.0);
	ptSet[6] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	transPt = Point2f(2.0 / 3.0, 2.0 / 3.0);
	ptSet[7] = applyTransform(_points, scaleMatrix, _points[0], transPt, length);

	const Point* a[1] = { &ptSet[0][0] };
	const Point* b[1] = { &ptSet[1][0] };
	const Point* c[1] = { &ptSet[2][0] };
	const Point* d[1] = { &ptSet[3][0] };
	const Point* e[1] = { &ptSet[4][0] };
	const Point* f[1] = { &ptSet[5][0] };
	const Point* g[1] = { &ptSet[6][0] };
	const Point* h[1] = { &ptSet[7][0] };
	int npt[] = { 4 };

	int colorG = 255 * (1.0 - (_level + 1) / 10.0);
	fillPoly(canvas, a, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, b, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, c, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, d, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, e, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, f, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, g, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, h, npt, 1, Scalar(0, colorG, 255 - colorG), 8);

	if (_level < 3)
	{
		draw(ptSet[0], _level + 1);
		draw(ptSet[1], _level + 1);
		draw(ptSet[2], _level + 1);
		draw(ptSet[3], _level + 1);
		draw(ptSet[4], _level + 1);
		draw(ptSet[5], _level + 1);
		draw(ptSet[6], _level + 1);
		draw(ptSet[7], _level + 1);
	}
}

int main(int argc, char** argv)
{
	canvas = Mat::zeros(480, 640, CV_8UC3);

	std::vector<Point> pts;
	pts.push_back(Point(120, 440));
	pts.push_back(Point(520, 440));
	pts.push_back(Point(520, 40));
	pts.push_back(Point(120, 40));

	//for (auto& p : pts)
	//{
	//	circle(canvas, p, 1, Scalar(0, 255, 0), 2, 8, 0);
	//}

	draw(pts, 0);

	imshow("canvas", canvas);

	waitKey();
	return 0;
}