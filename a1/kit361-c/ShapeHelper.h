#pragma once
#include "LineDrawer.h"
#include "Color.h"
#include <vector>
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

class Pane {
public:
	Point topLeft;
	Point botRight;
	Pane(Point pane_topLeft, Point pane_botRight) {
		topLeft = pane_topLeft;
		botRight = pane_botRight;
	}
};
class ShapeHelper {
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
		for (int p = 0; p < 50; p++) {
			lineDrawer->draw_line(20 + bx, 80 + p + by, 150 + bx, 150 + p + by, Color::WHITE);
			lineDrawer->draw_line(160 + p + bx, 270 + by, 240 + p + bx, 40 + by, Color::WHITE);
		}
	}

	static void draw_parallelogram_alt(Pane pane, std::vector<LineDrawer*> lineDrawers) {
		int bx = pane.topLeft.x;
		int by = pane.topLeft.y;
		int selector = 0;
		for (int p = 0; p < 50; p++) {
			lineDrawers[selector]->draw_line(20 + bx, 80 + p + by, 150 + bx, 150 + p + by, Color::WHITE);
			lineDrawers[selector]->draw_line(160 + p + bx, 270 + by, 240 + p + bx, 40 + by, Color::WHITE);
			selector = (selector + 1) % lineDrawers.size();
		}
	}

};