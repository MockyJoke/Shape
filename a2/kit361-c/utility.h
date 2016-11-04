#pragma once
#include <random>
class Color {
private:
	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned int> distribution_color;
public:
	static const unsigned int WHITE = 0xffffffff;
	static const unsigned int RED = 0xffff0000;
	static const unsigned int GREEN = 0xff00ff00;
	static const unsigned int BLUE = 0xff0000ff;
	static const unsigned int BLACK = 0xff000000;
	static unsigned int FromARGB(unsigned int a, unsigned int r, unsigned int g, unsigned int b) {
		uint32_t va = a << 24;
		uint32_t vr = r << 16;
		uint32_t vg = g << 8;
		uint32_t vb = b;
		uint32_t result = va + vr + vg + vb;
		return result;
	}

	static unsigned int FromLerp(unsigned int c1, unsigned int c2,double percent) {
		std::vector<int> com1 = Color(c1).GetARGBs();
		std::vector<int> com2 = Color(c2).GetARGBs();
		std::vector<unsigned int> com;

		com.push_back(255);
		for (int i = 1; i < 4; i++) {
			unsigned int y = com1[i] + percent * (com2[i] - com1[i]);
			com.push_back(y);
		}
		uint32_t c = Color::FromARGB(com[0], com[1], com[2], com[3]);
		return c;
	}


	unsigned color;
	Color() {
		distribution_color = std::uniform_int_distribution<unsigned int>(0, 255);
		color = Color::FromARGB(255,
			distribution_color(generator),
			distribution_color(generator),
			distribution_color(generator));
	}
	Color(unsigned int color) :color(color) {
	}
	std::vector<int> GetARGBs() {
		int a = color >> 24;
		int r = (color << 8) >> 24;
		int g = (color << 16) >> 24;
		int b = (color << 24) >> 24;
		return{ a,r,g,b };
	}
	unsigned int randNextColor() {
		color = Color::FromARGB(255,
			distribution_color(generator),
			distribution_color(generator),
			distribution_color(generator));
		return color;
	}
};



class Point2D
{
public:
	int x;
	int y;
	static Point2D GetMidPoint(Point2D p1, Point2D p2) {
		int x = (p1.x + p2.x) / 2;
		int y = (p1.y + p2.y) / 2;
		return Point2D(x, y);
	}
	Point2D() {
		x = 0;
		y = 0;
	}
	Point2D(int x, int y) {
		this->x = x;
		this->y = y;
	}
	Point2D GetPointBy_AngleAndDistance(double angleInDegree, int distance) {
		double angleInRadian = (angleInDegree / 360) * 2 * 3.141592653;
		int x = static_cast<int>(cos(angleInRadian) * distance);
		int y = static_cast<int>(sin(angleInRadian) * distance);
		return Point2D(this->x + x, this->y - y);
	}
};


class Pane {
public:
	Point2D topLeft;
	Point2D botRight;
	Pane(Point2D pane_topLeft, Point2D pane_botRight) {
		topLeft = pane_topLeft;
		botRight = pane_botRight;
	}
};