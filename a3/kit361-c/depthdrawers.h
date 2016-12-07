#pragma once
#include "projection.h"
#include "utility.h"
#include <algorithm>
using namespace std;

class DepthTriangleDrawer3D {
private:
	//void drawTriangle_blerp(std::vector<std::pair<Point2D, Color>> threePoints) {
	void drawTriangle_blerp(std::vector<ColorPoint3D> threePoints) {
		for (ColorPoint3D p : threePoints){
			scene->setPixel(p);
		}
		std::sort(threePoints.begin(), threePoints.end(), [](ColorPoint3D p1, ColorPoint3D p2) {
			return p1.y < p2.y;
		});
		if (threePoints[0].y == threePoints[1].y) {
			draw_horizontalTriangle_blerp(threePoints);
		}
		else if (threePoints[1].y != threePoints[2].y) {
			Line3D line(threePoints[0], threePoints[2]);
			auto p4 = line.GetHrizontalXPoint_blerp(threePoints[1].y);
			draw_horizontalTriangle_blerp({ p4,threePoints[1],threePoints[2] });
			draw_horizontalTriangle_blerp({ threePoints[0],threePoints[1],p4 });
		}
		else {
			draw_horizontalTriangle_blerp(threePoints);
		}

	}
	//void draw_horizontalTriangle_blerp(std::vector<std::pair<Point2D, Color>> threePoints) {
	void draw_horizontalTriangle_blerp(std::vector<ColorPoint3D> threePoints) {

		if (threePoints[1].y == threePoints[2].y) {
			Line3D leftLine(threePoints[0], threePoints[1]);
			Line3D rightLine(threePoints[0], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[1].y; y < threePoints[1].y ? y++ : y--) {
				auto leftPt = leftLine.GetHrizontalXPoint_blerp(y);
				auto rightPt = rightLine.GetHrizontalXPoint_blerp(y);
				horizontalFill_blerp(leftPt, rightPt);
			}
		}
		else {
			Line3D leftLine(threePoints[0], threePoints[2]);
			Line3D rightLine(threePoints[1], threePoints[2]);
			for (int y = threePoints[0].y; y != threePoints[2].y; y < threePoints[2].y ? y++ : y--) {
				auto leftPt = leftLine.GetHrizontalXPoint_blerp(y);
				auto rightPt = rightLine.GetHrizontalXPoint_blerp(y);
				horizontalFill_blerp(leftPt, rightPt);
			}
		}
	}
	/*void horizontalFill_blerp(int y, int x1, int x2, Color c1, Color c2) {
		for (int x = x1; x != x2; x1 < x2 ? x++ : x--) {
			double percent = static_cast<double>(x - x1) / (x2 - x1);
			Color c = Color::FromLerp(c1.color, c2.color, percent);
			scene->setPixel(x, y, c.color);
		}
		scene->setPixel(x2, y, c2.color);
	}*/
	void horizontalFill_blerp(ColorPoint3D p1, ColorPoint3D p2) {
		Line3D line(p1, p2);
		for (int x = p1.x; x != p2.x; p1.x < p2.x ? x++ : x--) {
			ColorPoint3D p = line.GetVerticalXPoint_blerp(x);
			scene->setPixel(p);
		}
		scene->setPixel(p2);
	}

public :
	DepthScene* scene;
	Camera* camera;
	DepthTriangleDrawer3D (DepthScene* scene, Camera* camera) {
		this->scene = scene;
		this->camera = camera;
	}
	void drawTriangle3D(vector<ColorPoint3D> threePoints, Matrix transMatrix) {
		Matrix perspecMatrix = Matrix::GetPerspectiveMatrix(camera);
		Matrix expand = Matrix::GetScaleMatrix(0.5*(camera->x_high - camera->x_low),0.5*( camera->y_high - camera->y_low), 1);
		perspecMatrix.PrintMatrix();
		transMatrix = expand*(perspecMatrix *transMatrix);
		transMatrix = scene->GetDrawTransformMatrix()*transMatrix;
		//transMatrix.PrintMatrix();
		vector<ColorPoint3D> transformedPoints;
		
		Color c;
		//c.reSeed();
		//transMatrix.PrintMatrix();
		for (auto& p : threePoints) {
			//p.color = c.randNextColor();

			Matrix m = transMatrix* p.GetMatrix();
			m.PrintMatrix();
			ColorPoint3D point = ColorPoint3D(m, p.color);
			point.z = (1-m.data[2][0]/ m.data[3][0])*(camera->z_far - camera->z_near);
			point.color = Color::FromLerp(Color::WHITE, Color::BLACK, point.z / 200.0);

			transformedPoints.push_back(point);
		}
		
		drawTriangle_blerp(transformedPoints);
	}


};