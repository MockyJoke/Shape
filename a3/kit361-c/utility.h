#pragma once
#include <random>
#include <tuple>
#ifdef _WINDOWS
#include "windows.h"
#endif // _WINDOWS
using namespace std;
int inline lerp(int v1, int v2, double percent) {
	return (int)round(v1 + percent * (v2 - v1));
}
class Camera {
public:
	double x_low;
	double y_low;
	double x_high;
	double y_high;
	double z_near;
	double z_far;
	//void setProjectionMatrix(const float &angleOfView, const float &near, const float &far, Matrix &M)
	//{
	//	Matrix M(4,4);
	//	// set the basic projection matrix
	//	float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
	//	M[0][0] = scale; // scale the x coordinates of the projected point 
	//	M[1][1] = scale; // scale the y coordinates of the projected point 
	//	M[2][2] = -far / (far - near); // used to remap z to [0,1] 
	//	M[3][2] = -far * near / (far - near); // used to remap z [0,1] 
	//	M[2][3] = -1; // set w = -z 
	//	M[3][3] = 0;
	//}
};
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
			unsigned int y = (uint32_t)round(com1[i] + percent * (com2[i] - com1[i]));
			com.push_back(y);
		}
		uint32_t c = Color::FromARGB(com[0], com[1], com[2], com[3]);
		return c;
	}


	unsigned int color;
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
	void reSeed() {
		std::random_device r;
		generator = std::default_random_engine(r());
	}
};



class Matrix {
public:
	double** data;
	int Cols;
	int Rows;
	Matrix(int rows, int cols) {
		Rows = rows;
		Cols = cols;
		data = new double*[rows];
		for (int i = 0; i < rows; i++) {
			data[i] = new double[cols];
			for (int j = 0; j < cols; j++) {
				data[i][j] = 0;
			}
		}
	}
	static Matrix GetIdentityMatrix(int n) {
		Matrix m(n, n);
		for (int i = 0; i < n; i++) {
			m.data[i][i] = 1;
		}
		return m;
	}

	static Matrix Multiply(Matrix m1, Matrix m2) {
		//m1.PrintMatrix();
		//m2.PrintMatrix();
		if (m1.Cols != m2.Rows) {
			return Matrix(0, 0);
		}
		Matrix m(m1.Rows, m2.Cols);

		for (int i = 0; i < m1.Rows; i++) {
			for (int j = 0; j < m2.Cols; j++) {
				double cell = 0;
				for (int k = 0; k < m1.Cols; k++) {
					cell += m1.data[i][k] * m2.data[k][j];
				}
				m.data[i][j] = cell;
			}
		}
		//m.PrintMatrix();
		return m;
	}
	static Matrix GetScaleMatrix(double sx, double sy, double sz) {
		Matrix m = GetIdentityMatrix(4);
		m.data[0][0] = sx;
		m.data[1][1] = sy;
		m.data[2][2] = sz;
		return m;
	}
	static Matrix GetTranslateMatrix(double vx, double vy, double vz) {
		Matrix m = GetIdentityMatrix(4);
		m.data[0][3] = vx;
		m.data[1][3] = vy;
		m.data[2][3] = vz;
		return m;
	}
	static Matrix GetRotateMatrix(char axis, double degree) {
		degree = degree *3.141592653 / 180.0;
		Matrix m = GetIdentityMatrix(4);
		switch (axis) {
		case 'X':
			m.data[1][1] = cos(degree);
			m.data[1][2] = sin(degree)*-1;
			m.data[2][1] = sin(degree);
			m.data[2][2] = cos(degree);
			break;
		case 'Y':
			m.data[0][0] = cos(degree);
			m.data[2][0] = sin(degree)*-1;
			m.data[0][2] = sin(degree);
			m.data[2][2] = cos(degree);
			break;
		case 'Z':
			m.data[0][0] = cos(degree);
			m.data[0][1] = sin(degree)*-1;
			m.data[1][0] = sin(degree);
			m.data[1][1] = cos(degree);
			break;
		}
		return m;
	}

	static Matrix GetPerspectiveMatrix(Camera* cam) {
		Matrix m(4, 4);
		m.data[0][0] = cam->z_near * 2 / (cam->x_high - cam->x_low);
		m.data[0][2] = (cam->x_high + cam->x_low) / (cam->x_high - cam->x_low);

		m.data[1][1] = (cam->z_near * 2) / (cam->y_high - cam->y_low);
		m.data[1][2] = (cam->y_high + cam->y_low) / (cam->y_high - cam->y_low);

		m.data[2][2] = -1.0* (cam->z_far + cam->z_near) / (cam->z_far - cam->z_near);
		m.data[2][3] = (-2.0 *cam->z_far * cam->z_near) / (cam->z_far - cam->z_near);

		m.data[3][2] = -1;
		//m.PrintMatrix();
		return m;
	}

	Matrix operator* (const Matrix& m) {
		return Multiply(*this, m);
	}
	void PrintMatrix() {
#ifdef _WINDOWS
		OutputDebugString(L"-------------------\r\n");
		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Cols; j++) {
				char buf[32];
				sprintf_s(buf, 32, "%f, ", data[i][j]);
				wstring str(buf, buf + 32);
				OutputDebugString(str.c_str());
			}
			OutputDebugString(L"\r\n");
		}
#endif // _WINDOWS
	}
};

class ViewBox {
public:
	int x_min;
	int x_max;
	int y_min;
	int y_max;
	int z_min;
	int z_max;
	ViewBox() {

	}
	ViewBox(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
		: x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max), z_min(z_min), z_max(z_max) {

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

class ColorPoint2D: public Point2D {
public:
	Color color;

	ColorPoint2D() :Point2D() {
		color = 0;
	}
	ColorPoint2D(int x, int y) :
		Point2D(x, y) {
		color = Color::BLACK;
	}

	ColorPoint2D(int x, int y,  Color color) :
		Point2D(x, y), color(color) {
	}
};

class Point3D
{
public:
	int x;
	int y;
	int z;
	int w;
	Point3D() {
		x = 0;
		y = 0;
		z = 0;
		w = 1;
	}

	Point3D(int x, int y,int z)
		:x(x),y(y),z(z) {
		w = 1;
	}

	Point3D(Matrix m){
		double W = m.data[3][0];
		x = m.data[0][0]/W;
		y = m.data[1][0]/W;
		z = m.data[2][0]/W;
		w = m.data[3][0]/W;
		
	}


	Matrix GetMatrix() {
		Matrix m(4, 1);
		m.data[0][0] = x;
		m.data[1][0] = y;
		m.data[2][0] = z;
		m.data[3][0] = w;
		return m;
	}

};

class ColorPoint3D : public Point3D
{
public:
	Color color;

	ColorPoint3D() :Point3D(){
		color = 0;
	}
	ColorPoint3D(int x, int y, int z ) :
		Point3D(x, y, z) {
		color = Color::GREEN;
	}

	ColorPoint3D(int x, int y, int z, Color color):
		Point3D(x,y,z),color(color){
	}
	ColorPoint3D(ColorPoint2D p, int z) {
		x = p.x;
		y = p.y;
		this->z = z;
		color = p.color;
	}

	ColorPoint3D(Matrix m,Color color=Color::BLUE):
		Point3D(m),color(color)
	{
		
	}
	ColorPoint2D Get2DPoint() {
		return ColorPoint2D(x, y);
	}
	
	bool isInViewBox(ViewBox box) {
		if (x<box.x_min || x>box.x_max) {
			return false;
		}
		if (y<box.y_min || y>box.y_max) {
			return false;
		}
		if (z<box.z_min || z>box.z_max) {
			return false;
		}
		return true;
	}

};


class Pane {
public:
	Point2D topLeft;
	Point2D botRight;
	Pane() {

	}
	Pane(Point2D pane_topLeft, Point2D pane_botRight) {
		topLeft = pane_topLeft;
		botRight = pane_botRight;
	}
};


class Line {
public:
	Point2D p1;
	Point2D p2;
	Color c1;
	Color c2;

	Line(Point2D p1, Point2D p2) :p1(p1), p2(p2) {
	}
	Line(Point2D p1, Point2D p2, Color c1, Color c2) :p1(p1), p2(p2), c1(c1), c2(c2) {
	}
	Line(pair<Point2D, Color> p1, pair<Point2D, Color> p2) 
		:Line(p1.first, p2.first, p1.second, p2.second) {
	}

	Point2D GetHrizontalXPoint(int y) {
		if (p1.x == p2.x) {
			return Point2D(p1.x, y);
		}
		double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
		double b = static_cast<double>(p1.y) - m * p1.x;
		int x = static_cast<int>((y - b) / m);
		return Point2D(x, y);
	}

	std::pair<Point2D,Color> GetHrizontalXPoint_blerp(int y) {
		if (p1.x == p2.x) {
			//return pair<Point2D, Color>(Point2D(p1.x, y), c1);
			double percent = static_cast<double>(y - p1.y) / (p2.y - p1.y);
			Color c = Color::FromLerp(c1.color, c2.color, percent);
			return pair<Point2D, Color>(Point2D(p1.x, y), c);
		}
		double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
		double b = static_cast<double>(p1.y) - m * p1.x;
		int x = static_cast<int>((y - b) / m);
		double percent = static_cast<double>(y - p1.y) / (p2.y - p1.y);
		Color c = Color::FromLerp(c1.color, c2.color, percent);
		return pair<Point2D, Color>(Point2D(x, y), c);
	}
};

class Line3D {
public:
	ColorPoint3D p1;
	ColorPoint3D p2;
	Line3D(ColorPoint3D p1, ColorPoint3D p2) :p1(p1), p2(p2) {
	}


	Point3D GetHrizontalXPoint(int y) {
		if (p1.x == p2.x) {
			return p1;
		}
		double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
		double b = static_cast<double>(p1.y) - m * p1.x;
		int x = static_cast<int>((y - b) / m);
		double percent = double(x - p1.x) / (p2.x - p1.x);
		int z = lerp(p1.z, p2.z, percent);
		return Point3D(x,y,z);
	}

	ColorPoint3D GetHrizontalXPoint_blerp(int y) {
		if (p1.x == p2.x) {
			//Line is vertical
			double percent = static_cast<double>(y - p1.y) / (p2.y - p1.y);
			Color c = Color::FromLerp(p1.color.color, p2.color.color, percent);
			int z = lerp(p1.z, p2.z, percent);
			return ColorPoint3D(p1.x, y, z, c);
		}
		double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
		double b = static_cast<double>(p1.y) - m * p1.x;
		int x = static_cast<int>((y - b) / m);
		double percent = static_cast<double>(y - p1.y) / (p2.y - p1.y);
		int z = lerp(p1.z, p2.z, percent);
		Color c = Color::FromLerp(p1.color.color, p2.color.color, percent);
		return ColorPoint3D(x, y, z, c);
	}

	ColorPoint3D GetVerticalXPoint_blerp(int x) {
		if (p1.y == p2.y) {
			//Line is horizontal
			double percent = static_cast<double>(x - p1.x) / (p2.x - p1.x);
			Color c = Color::FromLerp(p1.color.color, p2.color.color, percent);
			int z = lerp(p1.z, p2.z, percent);
			return ColorPoint3D(x, p1.y, z, c);
		}
		double m = static_cast<double>(p2.y - p1.y) / static_cast<double>(p2.x - p1.x);
		double b = static_cast<double>(p1.y) - m * p1.x;
		int y = static_cast<int>(m*x + b);
		double percent = static_cast<double>(x - p1.x) / (p2.x - p1.x);
		int z = lerp(p1.z, p2.z, percent);
		Color c = Color::FromLerp(p1.color.color, p2.color.color, percent);
		return ColorPoint3D(x, y, z, c);
	}
};
class MeshNet {
public:
	ColorPoint3D** data;
	int Rows;
	int Cols;
	MeshNet(int rows,int cols) {
		Rows = rows;
		Cols = cols;
		data = new ColorPoint3D*[Rows];
		for (int i = 0; i < Rows; i++) {
			data[i] = new ColorPoint3D[Cols];
		}
	}
	~MeshNet() {
		for (int i = 0; i < Rows; i++) {
			delete[] data[i];
		}
		delete[] data;
	}
};

