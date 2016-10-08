#pragma once
#include "LineDrawer.h"
#include "Color.h"
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include<cmath>
class Point
{

public:
	int x;
	int y;
	static Point GetMidPoint(Point p1, Point p2) {
		int x = (p1.x + p2.x) / 2;
		int y = (p1.y + p2.y) / 2;
		return Point(x, y);
	}
	Point() {
		x = 0;
		y = 0;
	}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point GetPointBy_AngleAndDistance(double angleInDegree, int distance) {
		double angleInRadian = (angleInDegree / 360) * 2 * PI;
		int x = static_cast<int>(cos(angleInRadian) * distance);
		int y = static_cast<int>(sin(angleInRadian) * distance);
		return Point(this->x + x, this->y - y);
	}

};

class Line {
public:
	Point p1;
	Point p2;
	Line(Point p1, Point p2) :p1(p1), p2(p2) {

	}

	Point GetHrozontalXPoint(int y) {
		/*std::vector<Point> pts = { p1,p2 };
		std::sort(pts.begin(), pts.end(), [](Point p1, Point p2) {
			return p1.y < p2.y;
		});
		int dy = pts[1].y - pts[0].y;
		double size = (y - pts[0].y) / static_cast<double>(pts[0].y);
		int dx= abs(pts[0])*/
		if (p1.x == p2.x) {
			return Point(p1.x, y);
		}
		double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
		double b = static_cast<double>(p1.y) - m * p1.x;
		int x = static_cast<int>((y - b) / m);
		return Point(x, y);
	}
};

class Pane {
public:
	Point topLeft;
	Point botRight;
	Pane(Point pane_topLeft, Point pane_botRight) {
		topLeft = pane_topLeft;
		botRight = pane_botRight;
	}
};

class TriangleDrawer {
private:
	Drawable* _drawable;
public:
	TriangleDrawer(Drawable* drawable)
		:_drawable(drawable) {

	}
	void drawTriangle(std::vector<Point> threePoints, unsigned int color) {
		std::sort(threePoints.begin(), threePoints.end(), [](Point p1, Point p2) {
			return p1.y < p2.y;
		});
		if (threePoints[1].y != threePoints[2].y) {
			
			Line line(threePoints[0], threePoints[2]);
			Point p4 = line.GetHrozontalXPoint(threePoints[1].y);
			draw_horizontalTriangle({ p4,threePoints[1],threePoints[2] }, color);
			draw_horizontalTriangle({ threePoints[0],threePoints[1],p4 }, color);
			
		}
		else {
			draw_horizontalTriangle({ threePoints[0],threePoints[1],threePoints[2] }, color);

		}

	}
	void draw_horizontalTriangle(std::vector<Point> threePoints, unsigned int color) {
		if (threePoints[1].y == threePoints[2].y) {
			Line leftLine(threePoints[0], threePoints[1]);
			Line rightLine(threePoints[0], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[1].y; y < threePoints[1].y ? y++ : y--) {
				Point leftPt = leftLine.GetHrozontalXPoint(y);
				Point rightPt = rightLine.GetHrozontalXPoint(y);
				horizontalFill(y, leftPt.x, rightPt.x, color);
			}
		}
		else {
			Line leftLine(threePoints[0], threePoints[2]);
			Line rightLine(threePoints[1], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[2].y; y < threePoints[2].y ? y++ : y--) {
				Point leftPt = leftLine.GetHrozontalXPoint(y);
				Point rightPt = rightLine.GetHrozontalXPoint(y);
				horizontalFill(y, leftPt.x, rightPt.x, color);
			}
		}
		
	}
	void horizontalFill(int y, int x1, int x2, unsigned int color) {
		for (int x = x1; x != x2; x1 < x2 ? x++ : x--) {
			_drawable->setPixel(x, y, color);
		}
	}
};
class ShapeHelper {
private:
public:
	static const int STARBURST_INTERVAL = 4;
	static void draw_starBurst(Pane pane, LineDrawer* lineDrawer) {
		int degree = 0;
		Point pane_center = Point::GetMidPoint(pane.topLeft, pane.botRight);
		while (degree < 360) {
			Point endPoint = pane_center.GetPointBy_AngleAndDistance(degree, 125);
			lineDrawer->draw_line(pane_center.x, pane_center.y, endPoint.x, endPoint.y, Color::WHITE);
			degree += STARBURST_INTERVAL;
		}
	}

	static void draw_starBurst_alt(Pane pane, std::vector<LineDrawer*> lineDrawers) {
		int degree = 0;
		Point pane_center = Point::GetMidPoint(pane.topLeft, pane.botRight);
		int selector = 0;
		while (degree < 360) {
			Point endPoint = pane_center.GetPointBy_AngleAndDistance(degree, 125);
			lineDrawers.at(selector)->draw_line(pane_center.x, pane_center.y, endPoint.x, endPoint.y, Color::WHITE);
			degree += STARBURST_INTERVAL;
			selector = (selector + 1) % lineDrawers.size();
		}
	}

	static void draw_parallelogram(Pane pane, LineDrawer* lineDrawer) {
		int bx = pane.topLeft.x;
		int by = pane.topLeft.y;
		for (int p = 0; p <= 50; p++) {
			lineDrawer->draw_line(20 + bx, 80 + p + by, 150 + bx, 150 + p + by, Color::WHITE);
			lineDrawer->draw_line(160 + p + bx, 270 + by, 240 + p + bx, 40 + by, Color::WHITE);
		}
	}

	static void draw_parallelogram_alt(Pane pane, std::vector<LineDrawer*> lineDrawers) {
		int bx = pane.topLeft.x;
		int by = pane.topLeft.y;
		int selector = 0;
		for (int p = 0; p <= 50; p++) {
			lineDrawers[selector]->draw_line(20 + bx, 80 + p + by, 150 + bx, 150 + p + by, Color::WHITE);
			lineDrawers[selector]->draw_line(160 + p + bx, 270 + by, 240 + p + bx, 40 + by, Color::WHITE);
			selector = (selector + 1) % lineDrawers.size();
		}
	}

	static void draw_randomLine(Pane pane, LineDrawer* lineDrawer) {
		int bx = pane.topLeft.x;
		int by = pane.topLeft.y;
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution_coord(0, 299);
		std::uniform_int_distribution<unsigned int> distribution_color(0, 255);

		for (int i = 0; i <= 120; i++) {

			unsigned int color = Color::FromARGB(255, 
				distribution_color(generator),
				distribution_color(generator), 
				distribution_color(generator));


			lineDrawer->draw_line(distribution_coord(generator) +bx, 
				distribution_coord(generator) + by, 
				distribution_coord(generator) + bx, 
				distribution_coord(generator) + by, 
				color);
		}
	}

	static void draw_randomLine_alt(Pane pane, std::vector<LineDrawer*> lineDrawers) {
		int bx = pane.topLeft.x;
		int by = pane.topLeft.y;
		std::default_random_engine generator;

		std::uniform_int_distribution<int> distribution_coord(0, 299);
		std::uniform_int_distribution<unsigned int> distribution_color(0, 255);
		int selector = 0;
		for (int i = 0; i <= 120; i++) {

			unsigned int color = Color::FromARGB(255,
				distribution_color(generator),
				distribution_color(generator),
				distribution_color(generator));


			lineDrawers[selector]->draw_line(distribution_coord(generator) + bx,
				distribution_coord(generator) + by,
				distribution_coord(generator) + bx,
				distribution_coord(generator) + by,
				color);
			selector = (selector + 1) % lineDrawers.size();
		}
	}

	static void draw_starburstTriangle(Pane pane, TriangleDrawer* triDrawer) {
		int degree = 0;
		Point pane_center = Point::GetMidPoint(pane.topLeft, pane.botRight);
		Color c;
		while (degree < 360) {
			Point endPoint1 = pane_center.GetPointBy_AngleAndDistance(degree, 125);
			degree += STARBURST_INTERVAL;
			Point endPoint2 = pane_center.GetPointBy_AngleAndDistance(degree, 125);
			//lineDrawer->draw_line(pane_center.x, pane_center.y, endPoint.x, endPoint.y, Color::WHITE);
			triDrawer->drawTriangle(std::vector<Point>{ pane_center,endPoint1,endPoint2 },c.randNextColor());
		}
	}
};

