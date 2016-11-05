#pragma once
#include "utility.h"
#include <vector>
#include "drawers.h"
#include "drawers3d.h"
#include <tuple>
using namespace std;
class ShapeHelper {
private:
public:
	static void draw_squaredShiftedTriangleLines(Pane pane, LineDrawer* lineDrawer) {
		int stepX = (pane.botRight.x - pane.topLeft.x) / 12;
		int stepY = (pane.botRight.y - pane.topLeft.y) / 12;
		int startX = static_cast<int>(1.5*stepX + pane.topLeft.x);
		int startY = static_cast<int>(1.5*stepY + pane.topLeft.y);
		Point2D startPt(startX, startY);
		Color c;
		std::default_random_engine generator;
		std::uniform_int_distribution<int> offset(-12, 12);
		Point2D pointSlots[10][10];
		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < 10; i++) {
				pointSlots[j][i] = Point2D(startX + i*stepX + offset(generator), startY + j*stepY + offset(generator));
			}
		}
		for (int j = 1; j < 10; j++) {
			for (int i = 1; i < 10; i++) {
				lineDrawer->draw_line_lerp(pointSlots[j - 1][i - 1], pointSlots[j - 1][i], c.randNextColor(), c.randNextColor());
				lineDrawer->draw_line_lerp(pointSlots[j - 1][i - 1], pointSlots[j][i - 1], c.randNextColor(), c.randNextColor());

				lineDrawer->draw_line_lerp(pointSlots[j][i - 1], pointSlots[j - 1][i], c.randNextColor(), c.randNextColor());

				lineDrawer->draw_line_lerp(pointSlots[j - 1][i], pointSlots[j][i], c.randNextColor(), c.randNextColor());
				lineDrawer->draw_line_lerp(pointSlots[j][i - 1], pointSlots[j][i], c.randNextColor(), c.randNextColor());
			}
		}
	}
	static void draw_squaredShiftedTriangle(Pane pane, TriangleDrawer* triDrawer) {
		int stepX = (pane.botRight.x - pane.topLeft.x) / 12;
		int stepY = (pane.botRight.y - pane.topLeft.y) / 12;
		int startX = static_cast<int>(1.5*stepX + pane.topLeft.x);
		int startY = static_cast<int>(1.5*stepY + pane.topLeft.y);
		Point2D startPt(startX, startY);
		Color c;
		std::default_random_engine generator;
		std::uniform_int_distribution<int> offset(-12, 12);
		pair<Point2D,Color> pointSlots[10][10];
		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < 10; i++) {
				pointSlots[j][i] = pair<Point2D, Color>(Point2D(startX + i*stepX + offset(generator), startY + j*stepY + offset(generator)), Color(c.randNextColor()));
			}
		}
		for (int j = 1; j < 10; j++) {
			for (int i = 1; i < 10; i++) {
				triDrawer->drawTriangle_blerp(
				{
					pointSlots[j - 1][i - 1],
					pointSlots[j - 1][i],
					pointSlots[j][i - 1]
				});

				triDrawer->drawTriangle_blerp(
				{
					pointSlots[j][i - 1],
					pointSlots[j - 1][i],
					pointSlots[j][i]
				});
			}
		}
	}
};

class ShapeHelper3D {
public:
	static void drawLine3d(Pane pane, LineDrawer3D* drawer, ColorPoint3D p1, ColorPoint3D p2, Matrix transMatrix, ColorMode colorMode) {

		transMatrix = GetDrawTransformMatrix(pane)*transMatrix;
		vector<ColorPoint3D> transformedPoints{
			ColorPoint3D(transMatrix * p1.GetMatrix()),
			ColorPoint3D(transMatrix * p2.GetMatrix())
		};
		drawer->drawLine(transformedPoints[0], transformedPoints[1], colorMode);
	}

	static void drawTriangle3D(Pane pane, TriangleDrawer3D* drawer, vector<ColorPoint3D> threePoints, Matrix transMatrix, ColorMode colorMode) {
		//ViewBox viewBox(-100, 100, -100, 100, 0, 200);
		//transMatrix = transMatrix*Matrix::GetRotateMatrix('Y', 0);

		transMatrix = GetDrawTransformMatrix(pane)*transMatrix;

		//transMatrix.PrintMatrix();
		vector<ColorPoint3D> transformedPoints;
		for (auto& p : threePoints) {
			transformedPoints.push_back(ColorPoint3D(Matrix::Multiply(transMatrix, p.GetMatrix())));
		}

		drawer->drawTriangle(transformedPoints, colorMode);
	}

	static Matrix GetDrawTransformMatrix(Pane pane) {
		ViewBox viewBox(-100, 100, -100, 100, 0, 200);
		Point2D mid = Point2D::GetMidPoint(pane.topLeft, pane.botRight);
		Matrix m_s = Matrix::GetScaleMatrix(650 / 200.0, -1 * 650 / 200.0, 1);
		Matrix m_t = Matrix::GetTranslateMatrix(mid.x, mid.y, 0);
		return m_t*m_s;
	}
};