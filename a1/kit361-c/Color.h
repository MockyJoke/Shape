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

	unsigned color;
	Color() {
		distribution_color = std::uniform_int_distribution<unsigned int>(0, 255);
		color = Color::FromARGB(255,
			distribution_color(generator),
			distribution_color(generator),
			distribution_color(generator));
	}
	Color(unsigned int color):color(color) {
	}
	std::vector<unsigned int> GetARGBs() {
		unsigned int a = color>>24;
		unsigned int r = (color<<8)>>24;
		unsigned int g = (color << 16) >> 24;
		unsigned int b = (color << 24) >> 24;
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