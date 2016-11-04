#pragma once
#include "drawable.h"
#include <cmath>
#include "utility.h"

class LineDrawer
{
protected:
	Drawable* _drawable;
public:
	LineDrawer(Drawable* drawable) {
		this->_drawable = drawable;
	}
	~LineDrawer() {

	}
	virtual void draw_line(int x1, int y1, int x2, int y2, unsigned int color) = 0;
	virtual void draw_line_lerp(int x1, int y1, int x2, int y2, unsigned int color1, unsigned int color2) = 0;
	virtual void draw_line_lerp(Point2D p1, Point2D p2, unsigned int color1, unsigned int color2) = 0;
};

class DDA_Drawer : public LineDrawer
{
public:
	DDA_Drawer(Drawable* drawable)
		: LineDrawer(drawable) {

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
		double m = static_cast<double>(y2 - y1) / static_cast<double>(x2 - x1);
		double b = y1 - m*x1;
		if (abs(m) <= 1) {
			for (int x = x1; x != x2; x1 < x2 ? x++ : x--)
			{
				double percent = static_cast<double>(x - x1) / (x2 - x1);
				double y = m * x + b;
				_drawable->setPixel(x, static_cast<int>(round(y)), Color::FromLerp(color1, color2, percent));
			}
		}
		else {
			for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
			{
				double percent = static_cast<double>(y - y1) / (y2 - y1);
				double x = (y - b) / m;
				_drawable->setPixel(static_cast<int>(round(x)), y, Color::FromLerp(color1, color2, percent));
			}
		}
	}
	void draw_line_lerp(Point2D p1, Point2D p2, unsigned int color1, unsigned int color2) {
		draw_line_lerp(p1.x, p1.y, p2.x, p2.y, color1, color2);
	}
};
