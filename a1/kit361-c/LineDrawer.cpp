#include "LineDrawer.h"
#include <cmath>
#include <algorithm>
#include "ShapeHelper.h"
LineDrawer::LineDrawer(Drawable* drawable)
{
	this->_drawable = drawable;
}


LineDrawer::~LineDrawer()
{
}

DDA_Drawer::DDA_Drawer(Drawable* drawable)
	:LineDrawer(drawable) {

}

DDA_Drawer::~DDA_Drawer() {

}

void DDA_Drawer::draw_line(int x1, int y1, int x2, int y2, unsigned int color) {
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

Bresenham_Drawer::Bresenham_Drawer(Drawable* drawable)
	:LineDrawer(drawable) {

}

Bresenham_Drawer::~Bresenham_Drawer() {

}

void Bresenham_Drawer::draw_line(int x1, int y1, int x2, int y2, unsigned int color) {
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);

	int two_dx = dx * 2;
	int two_dy = dy * 2;
	int t2_y = two_dy - two_dx;
	int t2_x = two_dx - two_dy;

	int err_y = y1 < y2 ? (two_dy - dx) : (two_dy - dx);
	int err_x = x1 < x2 ? (two_dx - dy) : (dy - two_dx);
	int y = y1;
	int x = x1;
	int step_y = y1 < y2 ? 1 : -1;
	int step_x = x1 < x2 ? 1 : -1;

	_drawable->setPixel(x1, y1, color);
	if (dx >= dy) {
		for (int x = x1; x != x2; x+=step_x) {
			if (err_y >= 0) {
				err_y += t2_y;
				y += step_y;
			}
			else {
				err_y += two_dy;
			}
			_drawable->setPixel(x, y, color);
		}
	}
	else {
		for (int y = y1; y != y2; y+=step_y) {
			if (err_x >= 0) {
				err_x += t2_x;
				x += step_x;
			}
			else {
				err_x += two_dx;
			}
			_drawable->setPixel(x, y, color);
		}
	}

}



AA_Drawer::AA_Drawer(Drawable* drawable)
	:LineDrawer(drawable) {

}

AA_Drawer::~AA_Drawer() {

}

void AA_Drawer::draw_line(int x1, int y1, int x2, int y2, unsigned int color) {
	double m = static_cast<double>(y2 - y1) / static_cast<double>(x2 - x1);
	double b = y1 - m*x1;
	LineEx line(PointEx(x1, y1), PointEx(x2, y2));
	if (abs(m) <= 1) {
		for (int x = x1; x != x2; x1 < x2 ? x++ : x--)
		{
			double y = m * x + b;
			SetPixelAA_X(PointEx(x,y), line, color);
			//_drawable->setPixel(x,r_y, color);

		}
	}
	else {
		for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
		{
			double x = (y - b) / m;
			//_drawable->setPixel(static_cast<int>(round(x)), y, color);
			SetPixelAA_Y(PointEx(x, y), line, color);
		}
	}
}

void AA_Drawer::SetPixelAA_X(PointEx basePoint, LineEx line, unsigned int color)
{

	double dis = line.GetDistaceToPointEx(PointEx(basePoint.x, round(basePoint.y)));
	double dis1 = line.GetDistaceToPointEx(PointEx(basePoint.x, round(basePoint.y) - 1));
	double dis2 = line.GetDistaceToPointEx(PointEx(basePoint.x, round(basePoint.y) + 1));

	if (round(basePoint.y) > basePoint.y) {
		_drawable->setPixel(round(basePoint.x), round(basePoint.y) + 1, getAAColor(dis2, color));
	}
	else if(round(basePoint.y) < basePoint.y) {
		_drawable->setPixel(round(basePoint.x), round(basePoint.y) - 1, getAAColor(dis2, color));

	}
	_drawable->setPixel(round(basePoint.x), round(basePoint.y), getAAColor(dis*1.2, color));
}

void AA_Drawer::SetPixelAA_Y(PointEx basePoint, LineEx line, unsigned int color)
{
	double dis = line.GetDistaceToPointEx(PointEx(basePoint.x, round(basePoint.y)));
	double dis1 = line.GetDistaceToPointEx(PointEx(basePoint.x + 1, round(basePoint.y)));
	double dis2 = line.GetDistaceToPointEx(PointEx(basePoint.x - 1, round(basePoint.y)));

	if (round(basePoint.x) > basePoint.x) {
		_drawable->setPixel(round(basePoint.x)+1, round(basePoint.y), getAAColor(dis2, color));
	}
	else if (round(basePoint.x) < basePoint.x) {
		_drawable->setPixel(round(basePoint.x)-1, round(basePoint.y), getAAColor(dis2, color));

	}
	_drawable->setPixel(round(basePoint.x), round(basePoint.y), getAAColor(dis*1.2, color));
}

unsigned int AA_Drawer::getAAColor(double distance, unsigned int color)
{
	double radius = sqrt(2);
	if (distance > radius) {
		return Color::BLACK;
	}
	unsigned int o_r = (color << 8) >> 24;
	unsigned int o_g = (color << 16) >> 24;
	unsigned int o_b = (color << 24) >> 24;

	unsigned int a_r = unsigned int(o_r*(1 - pow(distance *2.0 / 3.0, 2)));
	unsigned int a_g = unsigned int(o_g*(1 - pow(distance *2.0 / 3.0, 2)));
	unsigned int a_b = unsigned int(o_b*(1 - pow(distance *2.0 / 3.0, 2)));

	Color result = Color::FromARGB(255, a_r, a_g, a_b);
	return result.color;
}
