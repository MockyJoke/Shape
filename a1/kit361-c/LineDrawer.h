#pragma once
#include "drawable.h"
#include <cmath>
#define PI 3.141592653


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