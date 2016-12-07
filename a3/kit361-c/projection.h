#pragma once
#include "utility.h"
#include "drawable.h"
#include <tuple>

using namespace std;

class DepthBuffer {
public:
	pair<int, unsigned int>** data;
	int Cols;
	int Rows;
	DepthBuffer(int rows, int cols) {
		Rows = rows;
		Cols = cols;
		data = new pair<int, unsigned int>*[rows];
		for (int i = 0; i < rows; i++) {
			data[i] = new pair<int, unsigned int>[cols];
			for (int j = 0; j < cols; j++) {
				data[i][j] = pair<int, unsigned int>(INT_MAX, 0);
			}
		}
	}

	pair<int, unsigned int> GetAt(int x, int y) {
		return data[x][y];
	}

	bool SetAt(int x, int y, int z, unsigned int c, bool force = false) {
		if (force) {
			data[x][y] = pair<int, unsigned int>(z, c);
			return true;
		}
		else {
			if (z <= data[x][y].first) {
				data[x][y] = pair<int, unsigned int>(z, c);
				return true;
			}
			return false;
		}
	}

	~DepthBuffer() {
		for (int i = 0; i < Rows; i++) {
			delete[] data[i];
		}
		delete[] data;
		data = nullptr;
	}
};
class DepthScene {
	DepthBuffer* z_buffer;
	Pane pane;
	Drawable* drawable;
	Camera camera;
public:
	
	DepthScene(Pane pane, Drawable* drawable) {
		this->pane = pane;
		this->drawable = drawable;
		//z_buffer = new DepthBuffer(pane.botRight.y - pane.topLeft.y, pane.botRight.x - pane.topLeft.x);
		z_buffer = new DepthBuffer(pane.botRight.y, pane.botRight.x);

	}

	DepthScene(Point2D topLeft, Point2D botRight, Drawable* drawable) {
		pane.topLeft = topLeft;
		pane.botRight = botRight;
		this->drawable = drawable;
		z_buffer = new DepthBuffer(botRight.x - topLeft.x, botRight.y - topLeft.x);
	}

	~DepthScene() {
		delete z_buffer;
	}

	void setPixel(ColorPoint3D point) {
		if (IsPointInScene(point)) {
			if (z_buffer->SetAt(point.x, point.y, point.z, point.color.color)) {
				//drawable->setPixel(point.x, point.y, point.color.color);
			}
		}
	}
	void setPixel_Spect(ColorPoint3D point, Camera* camera) {
		Matrix perspecMatrix =  Matrix::GetPerspectiveMatrix(camera);
		perspecMatrix.PrintMatrix();
		ColorPoint3D pt(perspecMatrix*point.GetMatrix(), point.color);

		if (IsPointInScene(pt)) {
			point = pt;
			if (z_buffer->SetAt(point.x, point.y, point.z, point.color.color)) {
				//drawable->setPixel(point.x, point.y, point.color.color);
			}
		}
	}

	void render_all() {
		for (int i = pane.topLeft.x; i < pane.botRight.x; i++) {
			for (int j = pane.topLeft.y; j < pane.botRight.y; j++) {
				pair<int, unsigned int> value = z_buffer->GetAt(i, j);
				if (value.first != INT_MAX) {
					drawable->setPixel(i, j, value.second);
				}
			}
		}
	}

	bool IsPointInScene(Point3D point) {
		if (point.x<pane.topLeft.x || point.x>=pane.botRight.x) {
			return false;
		}
		else if (point.y<pane.topLeft.y || point.y>=pane.botRight.y) {
			return false;
		}
		else if (point.z<camera.z_near*-1 || point.z > camera.z_far*-1) {
			return false;
		}
		else {
			return true;
		}
	}
	
	void SetCamera(Camera cam) {
		camera = cam;
	}

	Matrix GetDrawTransformMatrix() {
		Point2D mid = Point2D::GetMidPoint(pane.topLeft, pane.botRight);
		Matrix m_s = Matrix::GetScaleMatrix(650 / 200.0, -1 * 650/ 200.0, 1);
		Matrix m_t = Matrix::GetTranslateMatrix(mid.x, mid.y, 0);
		return m_t;
	}
};