#pragma once
#include "drawable.h"
#include <cmath>
#include "utility.h"
#include <tuple>
#include <vector>
#include <functional>
#include "drawers.h"
using namespace std;

enum ColorMode { DepthCue_BW, DpethCue_Fix, Random };

class LineDrawer3D
{
protected:
	Drawable* _drawable;
public:
	LineDrawer3D(Drawable* _drawerable) {
		this->_drawable = _drawerable;
	}
	~LineDrawer3D() {

	}
	virtual void drawLine(ColorPoint3D p1, ColorPoint3D p2, ColorMode colorMode) = 0;
};

class DDA_Drawer3D : public LineDrawer3D {
private:
public:
	DDA_Drawer3D(Drawable* _drawerable) :
		LineDrawer3D(_drawerable) {

	}

	void drawLine(ColorPoint3D p1, ColorPoint3D p2, ColorMode colorMode) {
		DDA_Drawer drawer2d(_drawable);
		Color c;
		Color c1;
		Color c2;
		if (colorMode == ColorMode::DepthCue_BW) {
			c1.color = Color::FromLerp(Color::WHITE, Color::BLACK, p1.z / 200.0);
			c2.color = Color::FromLerp(Color::WHITE, Color::BLACK, p2.z / 200.0);
		}
		else if (colorMode == ColorMode::DpethCue_Fix) {
			c1.color = Color::FromLerp(Color::RED, Color::GREEN, p1.z / 200.0);
			c2.color = Color::FromLerp(Color::RED, Color::GREEN, p2.z / 200.0);
		} else {
			c.reSeed();
			c1.color = c.randNextColor();
			c2.color = c.randNextColor();
		}
		vector<ColorPoint2D> points2D{
			ColorPoint2D(p1.x, p1.y, c1),
			ColorPoint2D(p2.x, p2.y, c2)
		};
		
		drawer2d.draw_line_lerp(points2D[0], points2D[1]);
	}
};

class TriangleDrawer3D {
private:
	Drawable* _drawerable;
public:
	TriangleDrawer3D(Drawable* _drawerable) :_drawerable(_drawerable) {

	}

	void drawTriangle(vector<ColorPoint3D> points, ColorMode colorMode) {
		TriangleDrawer drawer2d(_drawerable);
		vector<ColorPoint2D> points2D;
		Color c;
		for (auto& p3D : points) {
			c.reSeed();
			if (colorMode == ColorMode::DepthCue_BW) {
				c=Color::FromLerp(Color::WHITE, Color::BLACK, p3D.z / 200.0);
			}
			else if (colorMode == ColorMode::DpethCue_Fix) {
				c = Color::FromLerp(Color::RED, Color::GREEN, p3D.z / 200.0);
			}
			else {
				c = c.randNextColor();
			}

			ColorPoint2D p2D(p3D.x, p3D.y, c);
			points2D.push_back(p2D);
		}

		drawer2d.drawTriangle_blerp(points2D);
	}
};