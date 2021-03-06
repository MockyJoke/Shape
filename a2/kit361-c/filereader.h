#pragma once
#include <string>
#include <stack>
#include <fstream>
#include "stringhelper.h"
#include "drawers3d.h"
#include <cmath>
#include "utility.h"
#include "shapeHelper.h"
using namespace std;

class MeshReader {
private:
	ifstream* fileStream;
public:
	MeshReader() {
		fileStream = nullptr;
	}
	MeshReader(ifstream* fileStream) {
		this->fileStream = fileStream;
	}
	void Run(Pane pane, Drawable* _drawable, ColorMode colorMode = ColorMode::DepthCue_BW) {
		string lineBuf;
		Matrix transMatrix = Matrix::GetIdentityMatrix(4);

		// Get Cols
		getline(*fileStream, lineBuf);
		string line = trim(lineBuf);
		auto l = split(line, ' ');
		int cols = std::stoi(line);
		// Get Rows
		getline(*fileStream, lineBuf);
		line = trim(lineBuf);
		int rows = std::stoi(line);
		MeshNet mesh(rows, cols);

		while (!fileStream->eof()) {
			getline(*fileStream, lineBuf);
			string line = trim(lineBuf);
			if (line[0] == '#' || line.length() == 0) {
				continue;
			}
			bool found = false;
			string buffer;
			int num[3];
			int index = 0;
			for (int i = 0; i < line.length(); i++) {
				if (isspace(line[i])) {
					if (found) {
						found = false;
						num[index % 3] = stoi(buffer);
						buffer = "";
						index++;
						if (index != 0 && index % 3 == 0) {
							ColorPoint3D p(num[0], num[1], num[2]);
							mesh.data[index / mesh.Cols][index% mesh.Cols] = p;
						}
					}
				}
				else {
					found = true;
					buffer = buffer + line[i];

				}
			}
		}
	}
};
enum DrawMode { Fill, Wireframe };
class SimpReader{
private:
	ifstream* fileStream;
public:
	SimpReader() {
		fileStream = nullptr;
	}

	SimpReader(ifstream* fileStream) {
		this->fileStream = fileStream;
	}

	void Run(Pane pane,Drawable* _drawable, ColorMode colorMode = ColorMode::DepthCue_BW) {
		DrawMode drawMode;
		string lineBuf;
		Matrix transMatrix = Matrix::GetIdentityMatrix(4);
		stack<Matrix> matrixStack = stack<Matrix>();
		while (!fileStream->eof()) {
			getline(*fileStream, lineBuf);
			string line = trim(lineBuf);
			if (line[0] == '#' || line.length() == 0) {
				continue;
			}

			if (line[0] == '{') {
				matrixStack.push(transMatrix);
				continue;
			}
			if (line[0] == '}') {
				transMatrix = matrixStack.top();
				matrixStack.pop();
				continue;
			}
			vector<string> words = split(line, ' ');
			if (words[0] == "scale") {
				Matrix m = Matrix::GetScaleMatrix(
					stod(words[1]),
					stod(words[2]),
					stod(words[3]));
				transMatrix = Matrix::Multiply(m, transMatrix);
			}
			else if (words[0] == "rotate") {
				Matrix m = Matrix::GetRotateMatrix(
					words[1][0],
					stod(words[2]));
				transMatrix = Matrix::Multiply(m, transMatrix);
			}
			else if (words[0] == "translate") {
				Matrix m = Matrix::GetTranslateMatrix(
					stod(words[1]),
					stod(words[2]),
					stod(words[3]));
				transMatrix = Matrix::Multiply(m, transMatrix);
			}
			else if (words[0] == "polygon") {
				auto params = extractParameters(line);
				ColorPoint3D p1 = ColorPoint3D(stoi(params[1][0]),
					stoi(params[1][1]),
					stoi(params[1][2]));
				ColorPoint3D p2 = ColorPoint3D(stoi(params[2][0]),
					stoi(params[2][1]),
					stoi(params[2][2]));
				ColorPoint3D p3 = ColorPoint3D(stoi(params[3][0]),
					stoi(params[3][1]),
					stoi(params[3][2]));
				TriangleDrawer3D drawer(_drawable);
				ShapeHelper3D::drawTriangle3D(pane, &drawer, { p1,p2,p3 }, transMatrix, colorMode);
				//drawable->updateScreen();   // you must call this to make the display change.
				//ShapeHelper3D::drawTriangle3D_Clip(pane, _drawable, { p1,p2,p3 }, transMatrix, colorMode);
			}
			else if (words[0] == "line") {

				auto params = extractParameters(line);
				ColorPoint3D p1 = ColorPoint3D(stoi(params[1][0]),
					stoi(params[1][1]),
					stoi(params[1][2]));
				ColorPoint3D p2 = ColorPoint3D(stoi(params[2][0]),
					stoi(params[2][1]),
					stoi(params[2][2]));
				DDA_Drawer3D drawer(_drawable);
				ShapeHelper3D::drawLine3d(pane, &drawer, p1, p2, transMatrix, colorMode);
				//ShapeHelper3D::drawLine3d_Clip(pane, _drawable, p1, p2, transMatrix, colorMode);
			}
			else if (words[0] == "mesh") {
				auto params = extractParameters(line);

				//std::ifstream meshFileStream;
				//meshFileStream.open(params[1][0].substr(1, params[1][0].length()-2));

				//MeshReader reader(&meshFileStream);
				//reader.Run(pane, _drawable, ColorMode::DepthCue_BW);
				//string param = line.substr(line.find_first_of(' '), line.length() - 7);
				//auto l = extractParameters(line);

			}
			else if (words[0] == "wire") {
				drawMode = DrawMode::Wireframe;

			}
			else if (words[0] == "filled") {
				drawMode = DrawMode::Fill;
			}
			
		}
	}
};

