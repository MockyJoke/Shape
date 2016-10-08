#pragma once
#include "drawable.h"
#include <cmath>
#define PI 3.141592653
class PointEx {
public:
	double x;
	double y;
	static double GetDistance(PointEx p1, PointEx p2) {
		double sum = pow((p2.x - p1.x), 2) + pow(p2.y - p1.y, 2);
		return sqrt(sum);
	}
	PointEx(double x, double y)
		:x(x), y(y)
	{

	}

};
class LineEx {
public:
	PointEx p1;
	PointEx p2;
	LineEx(PointEx p1, PointEx p2) :p1(p1), p2(p2)
	{

	}
	double GetDistaceToPointEx(PointEx p) {
		// (y1 – y2)x + (x2 – x1)y + (x1y2 – x2y1) = 0
		//double a = p1.y - p2.y;
		//double b = p2.x - p1.x;
		//double c = p1.x*p2.y - p2.x*p2.y;

		//double upper = abs(a*p.x + b*p.y + c);
		//double lower = sqrt(a*a + b*b);
		//double r1= upper / lower;

		double up = abs((p2.y - p1.y)*p.x - (p2.x - p1.x)*p.y + p2.x*p1.y - p2.y*p1.x);
		double lo = sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
		double r2 = up / lo;
		return r2;
	}
};

class LineDrawer
{
protected:
	Drawable* _drawable;
public:
	LineDrawer(Drawable* drawable);
	~LineDrawer();
	virtual void draw_line(int x1, int y1, int x2, int y2, unsigned int color) = 0;
};

class DDA_Drawer : public LineDrawer
{
public:
	DDA_Drawer(Drawable* drawable);
	~DDA_Drawer();
	void draw_line(int x1, int y1, int x2, int y2, unsigned int color);
};


class  Bresenham_Drawer : public LineDrawer {
public:
	Bresenham_Drawer(Drawable* drawable);
	~Bresenham_Drawer();
	void draw_line(int x1, int y1, int x2, int y2, unsigned int color);
};

class  AA_Drawer : public LineDrawer {
private:
	void SetPixelAA_X(PointEx basePoint,LineEx line, unsigned int color);
	void SetPixelAA_Y(PointEx basePoint, LineEx line, unsigned int color);
public:
	AA_Drawer(Drawable* drawable);
	~AA_Drawer();
	void draw_line(int x1, int y1, int x2, int y2, unsigned int color);
private:
	unsigned int getAAColor(double distance, unsigned int color);
};

