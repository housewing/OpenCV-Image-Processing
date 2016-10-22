#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
 
using namespace cv;
using namespace std;

struct Tri{
    Point p1;
	Point p2;
	Point p3;
};
vector<Tri> TriSet;

bool isInsideTri(Tri subTri, Point pt) {
	//Mat im = Mat::zeros(512,512,CV_8UC3);
	vector<Point> tmpPoint;
	tmpPoint.push_back(subTri.p1);
	tmpPoint.push_back(subTri.p2);
	tmpPoint.push_back(subTri.p3);

	const cv::Point *pts = (const cv::Point*) Mat(tmpPoint).data;
	int npts = Mat(tmpPoint).rows;

	//std::cout << "Number of polygon vertices: " << npts << std::endl;
	
	if (pointPolygonTest(Mat(tmpPoint), pt, true) >= 0){
		//cout << "distance " << pointPolygonTest(Mat(tmpPoint), pt, 1) << endl;
		return true;
	}
	else
	{
		return false;
	}
}

bool isInsidePoly(vector<Point> subPoly, Point pt) {
	//Mat im = Mat::zeros(512,512,CV_8UC3);
	vector<Point> tmpPoint;
	for (unsigned int i = 0; i < subPoly.size(); i++)
	{
		tmpPoint.push_back(subPoly[i]);
	}

	const cv::Point *pts = (const cv::Point*) Mat(tmpPoint).data;
	int npts = Mat(tmpPoint).rows;

	//std::cout << "Number of polygon vertices: " << npts << std::endl;

	if (pointPolygonTest(Mat(tmpPoint), pt, true) >= 0){
		//cout << "distance " << pointPolygonTest(Mat(tmpPoint), pt, 1) << endl;
		return true;
	}
	else
	{
		return false;
	}
}

// Draw a single point
static void draw_point( Mat& img, Point2f fp, Scalar color )
{
    circle( img, fp, 2, color, CV_FILLED, CV_AA, 0 );
}
 
// Draw delaunay triangles
static void draw_delaunay( Mat& img, Subdiv2D& subdiv, Scalar delaunay_color )
{
    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point> pt(3);
    Size size = img.size();
    Rect rect(0,0, size.width, size.height);
 
	//cout << "triList " << triangleList.size() << endl;
    for( size_t i = 0; i < triangleList.size(); i++ )
    {
        Vec6f t = triangleList[i];
		//cout << "t " << t << endl;
        
		pt[0] = Point(cvRound(t[0]), cvRound(t[1]));
        pt[1] = Point(cvRound(t[2]), cvRound(t[3]));
        pt[2] = Point(cvRound(t[4]), cvRound(t[5]));
        //cout << "pt " << pt << endl;

        // Draw rectangles completely inside the image.
        if ( rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
        {
			Tri tmpTri;
			tmpTri.p1 = pt[0];
			tmpTri.p2 = pt[1];
			tmpTri.p3 = pt[2];
			TriSet.push_back(tmpTri);

            line(img, pt[0], pt[1], delaunay_color, 1, CV_AA, 0);
            line(img, pt[1], pt[2], delaunay_color, 1, CV_AA, 0);
            line(img, pt[2], pt[0], delaunay_color, 1, CV_AA, 0);
			//cout << "p[0] " << pt[0] << " p[1] " << pt[1] << " p[2] " << pt[2] << endl;
        }
    }
}
 
//Draw voronoi diagram
static void draw_voronoi(Mat& img, Subdiv2D& subdiv)
{
	vector<vector<Point2f> > facets;
	vector<Point2f> centers;
	subdiv.getVoronoiFacetList(vector<int>(), facets, centers);

	vector<Point> ifacet;
	vector<vector<Point> > ifacets(1);

	for (size_t i = 0; i < facets.size(); i++)
	{
		ifacet.resize(facets[i].size());
		for (size_t j = 0; j < facets[i].size(); j++)
		{
			ifacet[j] = facets[i][j];
		}

		Scalar color;
		color[0] = rand() & 255;
		color[1] = rand() & 255;
		color[2] = rand() & 255;
		fillConvexPoly(img, ifacet, color, 8, 0);

		ifacets[0] = ifacet;
		polylines(img, ifacets, true, Scalar(), 1, CV_AA, 0);
		circle(img, centers[i], 3, Scalar(), CV_FILLED, CV_AA, 0);
	}
}

static void drawStainedGlass( Mat& src, Mat& dst, Subdiv2D& subdiv )
{
	dst = src.clone();

	vector<vector<Point2f>> facets;
    vector<Point2f> centers;
    subdiv.getVoronoiFacetList(vector<int>(), facets, centers);
 
    vector<Point> ifacet;
    vector<vector<Point> > ifacets(1);

	Size size = src.size();
	Rect rect(0, 0, size.width, size.height);
	//cout << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << endl;
 
	bool isInside = false;
	cout << "facets " << facets.size() << endl;
	for (size_t i = 0; i < facets.size(); i++)
	{
		vector<Point> polyPoint;
		vector<float> poly_x;
		vector<float> poly_y;
		ifacet.resize(facets[i].size());
		for (size_t j = 0; j < facets[i].size(); j++)
		{
			ifacet[j] = facets[i][j];
			poly_x.push_back(ifacet[j].x);
			poly_y.push_back(ifacet[j].y);
			//cout << ifacet[j] << endl;
		}
		sort(poly_x.begin(), poly_x.end());
		sort(poly_y.begin(), poly_y.end());

		int topLeft_x = poly_x[0];
		int topLeft_y = poly_y[0];
		int bottomRight_x = poly_x[ifacet.size() - 1];
		int bottomRight_y = poly_y[ifacet.size() - 1];
		//cout << topLeft_x << " " << topLeft_y << " " << bottomRight_x << " " << bottomRight_y << endl;

		if (topLeft_x < rect.x) { topLeft_x = rect.x; }
		else { topLeft_x = topLeft_x; }

		if (topLeft_y < rect.y) { topLeft_y = rect.y; }
		else { topLeft_y = topLeft_y; }

		if (bottomRight_x > rect.width) { bottomRight_x = rect.width; }
		else { bottomRight_x = bottomRight_x; }

		if (bottomRight_y > rect.height) { bottomRight_y = rect.height; }
		else { bottomRight_y = bottomRight_y;}

		//cout << topLeft_x << " " << topLeft_y << " " << bottomRight_x << " " << bottomRight_y << endl;

		int count = 0;
		double bChannel = 0;
		double gChannel = 0;
		double rChannel = 0;
		for (int y = topLeft_y; y < bottomRight_y; y++)
		{
			uchar *input = src.ptr<uchar>(y);
			for (int x = topLeft_x; x < bottomRight_x; x++)
			{
				isInside = isInsidePoly(ifacet, Point(x, y));
				//cout << "isInside " << isInside << endl;
							
				if(isInside)
				{
					polyPoint.push_back(Point(x, y));
					count++;
					double b = input[x * 3];
					double g = input[x * 3 + 1];
					double r = input[x * 3 + 2];

					bChannel += b;
					gChannel += g;
					rChannel += r;
				}
			}
		}

		double tmpB = bChannel/count;
		double tmpG = gChannel/count;
		double tmpR = rChannel/count;
		//cout << "b: " << tmpB << " g: " << tmpG << " r: " << tmpR << endl;


		Vec3b pixelColor(tmpB,tmpG,tmpR);
		for (unsigned int a = 0; a < polyPoint.size(); a++)
		{
			dst.at<Vec3b>(polyPoint[a].y, polyPoint[a].x) = pixelColor;
		}
    }
}

static void drawPolygon(Mat& src, Mat& dst)
{
	cout << "Tri size " << TriSet.size() << endl;
	bool isInside = false;
	double tmpB = 0;
	double tmpG = 0;
	double tmpR = 0;
	for(unsigned int i = 0; i < TriSet.size(); i++)
	{
		//cout << TriSet[i].p1 << " " << TriSet[i].p2 << " " << TriSet[i].p3 << " " << endl;
		vector<Point> triPoint;
		vector<float> tri_x;
		vector<float> tri_y;

		tri_x.push_back(TriSet[i].p1.x);
		tri_x.push_back(TriSet[i].p2.x);
		tri_x.push_back(TriSet[i].p3.x);

		tri_y.push_back(TriSet[i].p1.y);
		tri_y.push_back(TriSet[i].p2.y);
		tri_y.push_back(TriSet[i].p3.y);
		sort(tri_x.begin(), tri_x.end());
		sort(tri_y.begin(), tri_y.end());

		int topLeft_x = tri_x[0];
		int topLeft_y = tri_y[0];
		int bottomRight_x = tri_x[2];
		int bottomRight_y = tri_y[2];
		//cout << "tlX " << topLeft_x << " tlY " << topLeft_y << " blX " << bottomRight_x << " blY " << bottomRight_y << endl;

		int count = 0;
		double bChannel = 0;
		double gChannel = 0;
		double rChannel = 0;
		for(int y = topLeft_y; y < bottomRight_y; y++)
		{
			uchar *input = src.ptr<uchar>(y);
			for(int x = topLeft_x; x < bottomRight_x; x++)
			{
				isInside = isInsideTri(TriSet[i], Point(x,y));
				//cout << "isInside " << isInside << endl;
				
				if(isInside)
				{
					triPoint.push_back(Point(x, y));
					count++;
					double b = input[x * 3];
					double g = input[x * 3 + 1];
					double r = input[x * 3 + 2];

					bChannel += b;
					gChannel += g;
					rChannel += r;
				}
			}
		}

		tmpB = bChannel/count;
		tmpG = gChannel/count;
		tmpR = rChannel/count;
		//cout << "b: " << tmpB << " g: " << tmpG << " r: " << tmpR << endl;

		Vec3b pixelColor(tmpB,tmpG,tmpR);
		for (unsigned int a = 0; a < triPoint.size(); a++)
		{
			dst.at<Vec3b>(triPoint[a].y, triPoint[a].x) = pixelColor;
		}
	}
}

int main( int argc, char** argv)
{
	unsigned seed = (unsigned)time(NULL);
	srand(seed);

    // Turn on animation while drawing triangles
    bool animate = true;
 
    // Define colors for drawing.
    Scalar delaunay_color(255,255,255), points_color(0, 0, 255);
     
    // Read in the image.
    Mat img = imread("data/04.png");
	Mat polygon = img.clone();
	Mat stainedglass = img.clone();
     
    // Rectangle to be used with Subdiv2D
    Size size = img.size();
    Rect rect(0, 0, size.width, size.height);
 
    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);
     
    // Create a vector of points.
    vector<Point2f> points;
	int arraySize = 50;
	int width = img.cols;
	int height = img.rows;

	for (int i = 0; i < arraySize; i++)
	{
		Point2f fp((float)(rand() % width), (float)(rand() % width));
		points.push_back(fp);
	}
	points.push_back(Point2f(0.0, 0.0));
	points.push_back(Point2f(0.0, height / 2));
	points.push_back(Point2f(0.0, height - 1));
	points.push_back(Point2f(width / 2, height - 1));
	points.push_back(Point2f(width - 1, height - 1));
	points.push_back(Point2f(width - 1, height / 2));
	points.push_back(Point2f(width - 1, 0.0));
	points.push_back(Point2f(width / 2, 0.0));

    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
        subdiv.insert(*it);
    }
     
    // Draw delaunay triangles
    draw_delaunay( img, subdiv, delaunay_color );
 
    // Draw points
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
    {
        draw_point(img, *it, points_color);
    }

	// Allocate space for Voronoi Diagram
	Mat img_voronoi = Mat::zeros(img.rows, img.cols, CV_8UC3);
     
	// Draw Voronoi diagram
	draw_voronoi(img_voronoi, subdiv);
 
	//Draw polygonArt
	drawPolygon(img, polygon);

	//Draw stainedglassArt
	drawStainedGlass(img, stainedglass, subdiv);

	// Show results.
	imshow("Delaunay Triangulation", img);
	imshow("Voronoi Diagram", img_voronoi);
	imshow("polygon", polygon);
	imshow("StainedGlass", stainedglass);

    waitKey(0);
    return 0;
}