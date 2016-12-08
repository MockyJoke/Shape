#pragma once
#include "drawable.h"
#include <cmath>
#include "utility.h"
#include <tuple>
#include <functional>
using namespace std;
class LineDrawer
{
protected:
	Drawable* _drawable;
public:
	LineDrawer(Drawable* _drawable) {
		this->_drawable = _drawable;
	}
	~LineDrawer() {

	}
	virtual void draw_line(int x1, int y1, int x2, int y2, unsigned int color) = 0;
	virtual void draw_line_lerp(int x1, int y1, int x2, int y2, unsigned int color1, unsigned int color2) = 0;
	virtual void draw_line_lerp(Point2D p1, Point2D p2, unsigned int color1, unsigned int color2) = 0;
	//virtual void draw_line_lerp_ex(int x1, int y1, int x2, int y2, unsigned int color1, unsigned int color2, function<void(int, int, unsigned int)> prePixelSetAction) = 0;
	//virtual void draw_line_lerp_ex(Point2D p1, Point2D p2, unsigned int color1, unsigned int color2, function<void(int, int, unsigned int)> prePixelSetAction) = 0;

};

class DDA_Drawer : public LineDrawer
{
public:
	DDA_Drawer(Drawable* _drawable)
		: LineDrawer(_drawable) {

	}
	~DDA_Drawer() {

	}
	void draw_line(int x1, int y1, int x2, int y2, unsigned int color) {
		double m = static_cast<double>(y2 - y1) / static_cast<double>(x2 - x1);
		double b = y1 - m*x1;
		if (abs(m) <= 1) {
			for (int x = x1; x != x2; x1 < x2 ? x++ : x--)
			{
				double y = m * x + b;
				_drawable->setPixel(x, static_cast<int>(round(y)), color);
			}
		}
		else {
			for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
			{
				double x = (y - b) / m;
				_drawable->setPixel(static_cast<int>(round(x)), y, color);
			}
		}
	}

	void draw_line_lerp(int x1, int y1, int x2, int y2, unsigned int color1, unsigned int color2) {
		if (x1 == x2) {
			// vertical line 
			for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
			{
				double percent = static_cast<double>(y - y1) / (y2 - y1);
				unsigned int color = Color::FromLerp(color1, color2, percent);

				_drawable->setPixel(x1, y, color);
			}
			return;
		}
		double m = static_cast<double>(y2 - y1) / static_cast<double>(x2 - x1);
		double b = y1 - m*x1;
		if (abs(m) <= 1) {
			for (int x = x1; x != x2; x1 < x2 ? x++ : x--)
			{
				double percent = static_cast<double>(x - x1) / (x2 - x1);
				double y = m * x + b;
				unsigned int color = Color::FromLerp(color1, color2, percent);
				_drawable->setPixel(x, static_cast<int>(round(y)), color);
			}
		}
		else {
			for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
			{
				double percent = static_cast<double>(y - y1) / (y2 - y1);
				double x = (y - b) / m;
				unsigned int color = Color::FromLerp(color1, color2, percent);

				_drawable->setPixel(static_cast<int>(round(x)), y,color);
			}
		}
	}
	void draw_line_lerp(Point2D p1, Point2D p2, unsigned int color1, unsigned int color2) {
		draw_line_lerp(p1.x, p1.y, p2.x, p2.y, color1, color2);
	}
	void draw_line_lerp(ColorPoint2D p1, ColorPoint2D p2) {
		draw_line_lerp(p1.x, p1.y, p2.x, p2.y, p1.color.color, p2.color.color);
	}
	/*
	void draw_line_lerp_ex(int x1, int y1, int x2, int y2, unsigned int color1, unsigned int color2, function<void(int, int, unsigned int)> prePixelSetAction) {
		double m = static_cast<double>(y2 - y1) / static_cast<double>(x2 - x1);
		double b = y1 - m*x1;
		if (abs(m) <= 1) {
			for (int x = x1; x != x2; x1 < x2 ? x++ : x--)
			{
				double percent = static_cast<double>(x - x1) / (x2 - x1);
				double y_d = m * x + b;
				unsigned int color = Color::FromLerp(color1, color2, percent);
				int y = static_cast<int>(round(y_d));
				if (prePixelSetAction != nullptr) {
					prePixelSetAction(x, y, color);
				}
				_drawable->setPixel(x, y, color);
			}
		}
		else {
			for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
			{
				double percent = static_cast<double>(y - y1) / (y2 - y1);
				double x_d = (y - b) / m;
				int x = static_cast<int>(round(x_d));
				unsigned int color = Color::FromLerp(color1, color2, percent);
				if (prePixelSetAction != nullptr) {
					prePixelSetAction(x, y, color);
				}
				_drawable->setPixel(x, y, color);
			}
		}
	}

	void draw_line_lerp_ex(Point2D p1, Point2D p2, unsigned int color1, unsigned int color2, function<void(int, int, unsigned int)> prePixelSetAction) {
		draw_line_lerp_ex(p1.x, p1.y, p2.x, p2.y, color1, color2, prePixelSetAction);
	}
	*/
};


class TriangleDrawer {
private:
	Drawable* _drawable;
	void draw_horizontalTriangle(std::vector<Point2D> threePoints, unsigned int color) {
		if (threePoints[1].y == threePoints[2].y) {
			Line leftLine(threePoints[0], threePoints[1]);
			Line rightLine(threePoints[0], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[1].y; y < threePoints[1].y ? y++ : y--) {
				Point2D leftPt = leftLine.GetHrizontalXPoint(y);
				Point2D rightPt = rightLine.GetHrizontalXPoint(y);
				horizontalFill(y, leftPt.x, rightPt.x, color);
			}
		}
		else {
			Line leftLine(threePoints[0], threePoints[2]);
			Line rightLine(threePoints[1], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[2].y; y < threePoints[2].y ? y++ : y--) {
				Point2D leftPt = leftLine.GetHrizontalXPoint(y);
				Point2D rightPt = rightLine.GetHrizontalXPoint(y);
				horizontalFill(y, leftPt.x, rightPt.x, color);
			}
		}
	}
	void horizontalFill(int y, int x1, int x2, unsigned int color) {
		for (int x = x1; x != x2; x1 < x2 ? x++ : x--) {
			_drawable->setPixel(x, y, color);
		}
		_drawable->setPixel(x2, y, color);

	}
	void horizontalFill_blerp(int y, int x1, int x2, Color c1, Color c2) {
		for (int x = x1; x != x2; x1 < x2 ? x++ : x--) {
			double percent = static_cast<double>(x - x1) / (x2 - x1);
			Color c = Color::FromLerp(c1.color, c2.color, percent);
			_drawable->setPixel(x, y, c.color);
		}
		_drawable->setPixel(x2, y, c2.color);

	}
	void draw_horizontalTriangle_blerp(std::vector<std::pair<Point2D,Color>> threePoints) {
		if (threePoints[1].first.y == threePoints[2].first.y) {
			Line leftLine(threePoints[0], threePoints[1]);
			Line rightLine(threePoints[0], threePoints[2]);
			for (int y = threePoints[0].first.y; y != threePoints[1].first.y; y < threePoints[1].first.y ? y++ : y--) {
				auto leftPt = leftLine.GetHrizontalXPoint_blerp(y);
				auto rightPt = rightLine.GetHrizontalXPoint_blerp(y);
				horizontalFill_blerp(y, leftPt.first.x, rightPt.first.x, leftPt.second, rightPt.second);
			}
		}
		else {
			Line leftLine(threePoints[0], threePoints[2]);
			Line rightLine(threePoints[1], threePoints[2]);
			for (int y = threePoints[0].first.y; y != threePoints[2].first.y; y < threePoints[2].first.y ? y++ : y--) {
				auto leftPt = leftLine.GetHrizontalXPoint_blerp(y);
				auto rightPt = rightLine.GetHrizontalXPoint_blerp(y);
				horizontalFill_blerp(y, leftPt.first.x, rightPt.first.x, leftPt.second, rightPt.second);
			}
		}


	}

public:
	TriangleDrawer(Drawable* _drawable)
		:_drawable(_drawable) {
	}
	void drawTriangle(std::vector<Point2D> threePoints, unsigned int color) {
		std::sort(threePoints.begin(), threePoints.end(), [](Point2D p1, Point2D p2) {
			return p1.y < p2.y;
		});
		if (threePoints[1].y != threePoints[2].y) {

			Line line(threePoints[0], threePoints[2]);
			Point2D p4 = line.GetHrizontalXPoint(threePoints[1].y);
			draw_horizontalTriangle({ p4,threePoints[1],threePoints[2] }, color);
			draw_horizontalTriangle({ threePoints[0],threePoints[1],p4 }, color);

		}
		else {
			draw_horizontalTriangle({ threePoints[0],threePoints[1],threePoints[2] }, color);
		}

	}

	void drawTriangle_blerp(std::vector<std::pair<Point2D, Color>> threePoints) {
		std::sort(threePoints.begin(), threePoints.end(), [](std::pair<Point2D, Color> p1, std::pair<Point2D, Color> p2) {
			return p1.first.y < p2.first.y;
		});
		if (threePoints[1].first.y != threePoints[2].first.y) {
			Line line(threePoints[0], threePoints[2]);
			auto p4 = line.GetHrizontalXPoint_blerp(threePoints[1].first.y);
			draw_horizontalTriangle_blerp({ p4,threePoints[1],threePoints[2] });
			draw_horizontalTriangle_blerp({ threePoints[0],threePoints[1],p4 });
		}
		else {
			draw_horizontalTriangle_blerp({ threePoints[0],threePoints[1],threePoints[2] });
		}

	}
	void drawTriangle_blerp(vector<ColorPoint2D> threePoints) {
		drawTriangle_blerp({
			pair<Point2D,Color>(Point2D(threePoints[0].x,threePoints[0].y),threePoints[0].color),
			pair<Point2D,Color>(Point2D(threePoints[1].x,threePoints[1].y),threePoints[1].color),
			pair<Point2D,Color>(Point2D(threePoints[2].x,threePoints[2].y),threePoints[2].color),
		});
	}
};