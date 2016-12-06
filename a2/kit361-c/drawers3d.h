#pragma once
#include "drawable.h"
#include <cmath>
#include "utility.h"
#include <tuple>
#include <vector>
#include <functional>
#include "drawers.h"
using namespace std;

enum ColorMode { DepthCue_BW, DepthCue_Fix, Random };

class LineDrawer3D
{
protected:
	Drawable* _drawable;
public:
	LineDrawer3D(Drawable* _drawable) {
		this->_drawable = _drawable;
	}
	~LineDrawer3D() {

	}
	virtual void drawLine(ColorPoint3D p1, ColorPoint3D p2, ColorMode colorMode) = 0;

};
class DDA_Drawer3D : public LineDrawer3D {
private:
public:
	DDA_Drawer3D(Drawable* _drawable) :
		LineDrawer3D(_drawable) {

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
		else if (colorMode == ColorMode::DepthCue_Fix) {
			c1.color = Color::FromLerp(Color::RED, Color::GREEN, p1.z / 200.0);
			c2.color = Color::FromLerp(Color::RED, Color::GREEN, p2.z / 200.0);
		}
		else {
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

class DDA_Clip_Drawer3D : public DDA_Drawer3D {
private:
	

	void project_Z_draw_lerp(ColorPoint3D p1, ColorPoint3D p2, Matrix drawTransMatrix, ViewBox viewBox) {
		int x1 = p1.x;
		int x2 = p2.x;
		int y1 = p1.y;
		int y2 = p2.y;
		if (x1 == x2) {
			// vertical line 
			for (int y = p1.y; y != y2; y1 < y2 ? y++ : y--)
			{
				double percent = static_cast<double>(y - y1) / (y2 - y1);
				int z = lerp(p1.z, p2.z, percent);
				ColorPoint3D p(x1, y, z);
				if (p.isInViewBox(viewBox)) {
					unsigned int color = Color::FromLerp(p1.color.color, p2.color.color, percent);
					ColorPoint3D targetPoint = ColorPoint3D(drawTransMatrix * p.GetMatrix());
					_drawable->setPixel(targetPoint.x, targetPoint.y, color);

				}
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
				int z = lerp(p1.z, p2.z, percent);
				ColorPoint3D p(x, (int)y, z);
				if (p.isInViewBox(viewBox)) {
					unsigned int color = Color::FromLerp(p1.color.color, p2.color.color, percent);
					ColorPoint3D targetPoint = ColorPoint3D(drawTransMatrix * p.GetMatrix());
					_drawable->setPixel(targetPoint.x, targetPoint.y, color);

				}
			}
		}
		else {
			for (int y = y1; y != y2; y1 < y2 ? y++ : y--)
			{
				double percent = static_cast<double>(y - y1) / (y2 - y1);
				double x = (y - b) / m;
				int z = lerp(p1.z, p2.z, percent);
				ColorPoint3D p((int)x, (int)y, z);
				if (p.isInViewBox(viewBox)) {
					unsigned int color = Color::FromLerp(p1.color.color, p2.color.color, percent);
					ColorPoint3D targetPoint = ColorPoint3D(drawTransMatrix * p.GetMatrix());
					_drawable->setPixel(targetPoint.x, targetPoint.y, color);

				}
			}
		}
	}
public:
	DDA_Clip_Drawer3D(Drawable* _drawable) :
		DDA_Drawer3D(_drawable) {

	}

	void drawLine_Clip(ColorPoint3D p1, ColorPoint3D p2, ColorMode colorMode, Matrix drawTransMatrix, ViewBox viewBox) {
		DDA_Drawer drawer2d(_drawable);
		Color c;
		Color c1;
		Color c2;
		if (colorMode == ColorMode::DepthCue_BW) {
			c1.color = Color::FromLerp(Color::WHITE, Color::BLACK, p1.z / 200.0);
			c2.color = Color::FromLerp(Color::WHITE, Color::BLACK, p2.z / 200.0);
		}
		else if (colorMode == ColorMode::DepthCue_Fix) {
			c1.color = Color::FromLerp(Color::RED, Color::GREEN, p1.z / 200.0);
			c2.color = Color::FromLerp(Color::RED, Color::GREEN, p2.z / 200.0);
		} else {
			c.reSeed();
			c1.color = c.randNextColor();
			c2.color = c.randNextColor();
		}
		vector<ColorPoint3D> points2D{
			ColorPoint3D(p1.x, p1.y,p1.z,c1),
			ColorPoint3D(p2.x, p2.y, p2.z,c2)
		};
		
		project_Z_draw_lerp(points2D[0], points2D[1], drawTransMatrix, viewBox);
	}
};



class TriangleDrawer3D {
private:
	Drawable* _drawable;
public:
	TriangleDrawer3D(Drawable* _drawable) :_drawable(_drawable) {

	}

	void drawTriangle(vector<ColorPoint3D> points, ColorMode colorMode) {
		TriangleDrawer drawer2d(_drawable);
		vector<ColorPoint2D> points2D;
		Color c;
		for (auto& p3D : points) {
			c.reSeed();
			if (colorMode == ColorMode::DepthCue_BW) {
				c=Color::FromLerp(Color::WHITE, Color::BLACK, p3D.z / 200.0);
			}
			else if (colorMode == ColorMode::DepthCue_Fix) {
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


class TriangleDrawer3D_Clip  {
private:
	Drawable* _drawable;
	ViewBox viewBox;
	Matrix* drawTransMatrix;
	//void horizontalFill_blerp_clip(int y, int x1, int x2, Color c1, Color c2) {
	void horizontalFill_blerp_clip( ColorPoint3D p1,ColorPoint3D p2) {

		for (int x = p1.x; x != p2.x; p1.x < p2.x ? x++ : x--) {
			double percent = static_cast<double>(x - p1.x) / (p2.x - p1.x);
			
			//_drawable->setPixel(x, y, c.color);
			int z = lerp(p1.z, p2.z, percent);
			ColorPoint3D p(x, p1.y, z);
			if (p.isInViewBox(viewBox)) {
				Color c = Color::FromLerp(p1.color.color, p2.color.color, percent);
				ColorPoint3D targetPoint = ColorPoint3D(*drawTransMatrix * p.GetMatrix());
				_drawable->setPixel(targetPoint.x, targetPoint.y, c.color);

			}
		}
		//_drawable->setPixel(x2, y, c2.color);
		//int z = lerp(p1.z, p2.z, 1);
		//ColorPoint3D p(x, y, z);
		if (p2.isInViewBox(viewBox)) {
			//Color c = Color::FromLerp(p1.color.color, p2.color.color, percent);
			ColorPoint3D targetPoint = ColorPoint3D(*drawTransMatrix * p2.GetMatrix());
			_drawable->setPixel(p2.x, p2.y, p2.color.color);

		}

	}
	void draw_horizontalTriangle_blerp_clip(std::vector<ColorPoint3D> threePoints) {
		if (threePoints[1].y == threePoints[2].y) {
			Line3D leftLine(threePoints[0], threePoints[1]);
			Line3D rightLine(threePoints[0], threePoints[2]);

			for (int y = threePoints[0].y; y != threePoints[1].y; y < threePoints[1].y ? y++ : y--) {
				auto leftPt = leftLine.GetHrizontalXPoint_blerp(y);
				auto rightPt = rightLine.GetHrizontalXPoint_blerp(y);
				horizontalFill_blerp_clip(leftPt, rightPt);
			}
		}
		else {
			Line3D leftLine(threePoints[0], threePoints[2]);
			Line3D rightLine(threePoints[1], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[2].y; y < threePoints[2].y ? y++ : y--) {
				auto leftPt = leftLine.GetHrizontalXPoint_blerp(y);
				auto rightPt = rightLine.GetHrizontalXPoint_blerp(y);
				horizontalFill_blerp_clip(leftPt, rightPt);
			}
		}


	}
	void drawTriangle_blerp_clip(vector<ColorPoint3D> points) {
		std::sort(points.begin(), points.end(), [](ColorPoint3D p1, ColorPoint3D p2) {
			return p1.y < p2.y;
		});
		if (points[1].y != points[2].y) {

			Line3D line(points[0], points[2]);
			ColorPoint3D p4 = line.GetHrizontalXPoint_blerp(points[1].y);
			draw_horizontalTriangle_blerp_clip({ p4,points[1],points[2] });
			draw_horizontalTriangle_blerp_clip({ points[0],points[1],p4 });

		}
		else {
			draw_horizontalTriangle_blerp_clip(points);
		}
	}
public:
	TriangleDrawer3D_Clip(Drawable* _drawable,ViewBox viewBox, Matrix* drawTransMatrix)
	{
		this->_drawable = _drawable;
		this->viewBox = viewBox;
		this->drawTransMatrix = drawTransMatrix;
	}

	void drawTriangle_clip(vector<ColorPoint3D> points, ColorMode colorMode) {
		//TriangleDrawer drawer2d(_drawerable);
		//vector<ColorPoint2D> points2D;
		Color c;
		for (auto& p3D : points) {
			c.reSeed();
			if (colorMode == ColorMode::DepthCue_BW) {
				p3D.color.color = Color::FromLerp(Color::WHITE, Color::BLACK, p3D.z / 200.0);
			}
			else if (colorMode == ColorMode::DepthCue_Fix) {
				p3D.color.color = Color::FromLerp(Color::RED, Color::GREEN, p3D.z / 200.0);
			}
			else {
				p3D.color.color = c.randNextColor();
			}

		}
		drawTriangle_blerp_clip(points);
	}
};