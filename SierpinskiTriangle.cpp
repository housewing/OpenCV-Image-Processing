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

Point applyRotation(Point _points, cv::Mat _rotationMatrix, cv::Point _reference)
{
	cv::Point tmp = _points;
	tmp.x = tmp.x - _reference.x;
	tmp.y = tmp.y - _reference.y;
	_points.x = (tmp.x * _rotationMatrix.at<float>(0, 0) + tmp.y * _rotationMatrix.at<float>(0, 1)) + _reference.x;
	_points.y = (tmp.x * _rotationMatrix.at<float>(1, 0) + tmp.y * _rotationMatrix.at<float>(1, 1)) + _reference.y;

	return _points;
}

Mat getRotation(float _rotation)
{
	cv::Mat rotationMatrix = cv::Mat::zeros(2, 2, CV_32FC1);
	rotationMatrix.at<float>(0, 0) = cos(_rotation);
	rotationMatrix.at<float>(0, 1) = -sin(_rotation);
	rotationMatrix.at<float>(1, 0) = sin(_rotation);
	rotationMatrix.at<float>(1, 1) = cos(_rotation);

	return rotationMatrix;
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

void draw(std::vector<Point> _pts, int _level)
{
	Mat scaleMatrix = getScale(0.5);
	Point2f lenPt;
	lenPt.x = _pts[0].x - _pts[1].x;
	lenPt.y = _pts[0].y - _pts[1].y;
	float length = std::sqrt(std::pow(lenPt.x, 2) + std::pow(lenPt.y, 2));
	Point2f transPt = Point2f(0.0, 0.0);
	std::vector<Point> first = applyTransform(_pts, scaleMatrix, _pts[0], transPt, length);

	transPt = Point2f(1.0 / 2.0, 0.0);
	std::vector<Point> second = applyTransform(_pts, scaleMatrix, _pts[0], transPt, length);

	transPt = Point2f(1.0 / 4.0, std::sqrt(3) / 4.0);
	std::vector<Point> third = applyTransform(_pts, scaleMatrix, _pts[0], transPt, length);

	//std::vector<Point> black;
	//black.push_back(third[0]);
	//black.push_back(third[1]);
	//black.push_back(first[1]);

	const Point* f[1] = { &first[0] };
	const Point* s[1] = { &second[0] };
	const Point* t[1] = { &third[0] };
	//const Point* b[1] = { &black[0] };
	int npt[] = { 3 };

	int colorG = 255 * (1.0 - (_level + 1) / 10.0);
	fillPoly(canvas, f, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, s, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	fillPoly(canvas, t, npt, 1, Scalar(0, colorG, 255 - colorG), 8);
	//fillPoly(canvas, b, npt, 1, Scalar(0, 0, 0), 8);

	if (_level < 5)
	{
		draw(first, _level + 1);
		draw(second, _level + 1);
		draw(third, _level + 1);
	}
}

int main(int argc, char** argv)
{
	canvas = Mat::zeros(480, 640, CV_8UC3);

	std::vector<Point> pts;
	pts.push_back(Point(120, 420));
	pts.push_back(Point(520, 420));

	float rotation = CV_PI / 180.0f * 60;
	Mat rotationMatrix = getRotation(rotation);
	Point tmpThird = applyRotation(pts[0], rotationMatrix, pts[1]);
	pts.push_back(tmpThird);

	draw(pts, 0);

	imshow("canvas", canvas);

	waitKey();
	return 0;
}