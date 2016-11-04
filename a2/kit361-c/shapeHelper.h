#pragma once
#include "utility.h"
#include <vector>
#include "drawers.h"

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
				lineDrawer->draw_line_lerp(pointSlots[j - 1][i - 1], pointSlots[j][i -1], c.randNextColor(), c.randNextColor());

				lineDrawer->draw_line_lerp(pointSlots[j][i - 1], pointSlots[j - 1][i], c.randNextColor(), c.randNextColor());

				lineDrawer->draw_line_lerp(pointSlots[j - 1][i], pointSlots[j][i], c.randNextColor(), c.randNextColor());
				lineDrawer->draw_line_lerp(pointSlots[j][i - 1], pointSlots[j][i], c.randNextColor(), c.randNextColor());
			}
		}
	}
};