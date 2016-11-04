#pragma once
#include "drawable.h"
#include <cmath>
#include "utility.h"
#include <tuple>
#include <vector>
#include <functional>
#include "drawers.h"
using namespace std;


class LineDrawer3D
{
protected:
	Drawable* _drawable;
public:
	LineDrawer3D(Drawable* drawable) {
		this->_drawable = drawable;
	}
	~LineDrawer3D() {

	}
	virtual void drawLine(ColorPoint3D p1, ColorPoint3D p2) = 0;
};

class DDA_Drawer3D : public LineDrawer3D{
private:
public:
	DDA_Drawer3D(Drawable* drawable) :
		LineDrawer3D(drawable) {

	}
	
	void drawLine(ColorPoint3D p1, ColorPoint3D p2) {

	}
};

class TriangleDrawer3D {
private:
	Drawable* drawable;
public:
	TriangleDrawer3D(Drawable* drawable) :drawable(drawable) {

	}

	void drawTriangle(vector<ColorPoint3D> points) {
		TriangleDrawer drawer2d(drawable);
		vector<ColorPoint2D> points2D;
		for (auto& p3D : points) {
			Color c =Color::FromLerp(Color::WHITE, Color::BLACK, p3D.z / 200.0);
			ColorPoint2D p2D(p3D.x, p3D.y, c);
			points2D.push_back(p2D);
		}

		drawer2d.drawTriangle_blerp(points2D);
	}
};