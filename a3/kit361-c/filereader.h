#pragma once
#include <string>
#include <stack>
#include <fstream>
#include "stringhelper.h"
#include "drawers3d.h"
#include <cmath>
#include "utility.h"
#include "shapeHelper.h"
#include "projection.h"
#include "depthdrawers.h"

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
enum RenderStyle { Phong, Gouraud, Flat };
class SimpReader {
private:
	ifstream* fileStream;
public:
	SimpReader() {
		fileStream = nullptr;
	}

	SimpReader(ifstream* fileStream) {
		this->fileStream = fileStream;
	}

	void Run(Pane pane, Drawable* _drawable, ColorMode colorMode = ColorMode::DepthCue_BW) {
		DepthScene scene(pane, _drawable);
		LightParameters lightParam;
		/*vector <LightSource> LightSources = {};
		SurfaceLight surfaceLight;
		AmbientLight ambientLight;*/
		Camera camera;
		DrawMode drawMode;
		RenderStyle renderStyle;
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
			//vector<string> words = split(line, ' ');
			vector<string> words = parseSimpLine(line);
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
				//m.PrintMatrix();
				//transMatrix.PrintMatrix();
				transMatrix = Matrix::Multiply(m, transMatrix);
				//transMatrix.PrintMatrix();
			}
			else if (words[0] == "translate") {
				Matrix m = Matrix::GetTranslateMatrix(
					stod(words[1]),
					stod(words[2]),
					stod(words[3]));
				transMatrix = Matrix::Multiply(m, transMatrix);
			}
			else if (words[0] == "polygon") {
				vector<vector<string>> params;
				vector<vector<double>> normals;
				bool hasNormal = false;
				if (contains(line, "[")) {
					hasNormal = true;
					normals = extractNormals_Polygon(line);
				}
				params = extractParameters(line);

				ColorPoint3D p1 = ColorPoint3D(stoi(params[1][0]),
					stoi(params[1][1]),
					stoi(params[1][2]));
				ColorPoint3D p2 = ColorPoint3D(stoi(params[2][0]),
					stoi(params[2][1]),
					stoi(params[2][2]));
				ColorPoint3D p3 = ColorPoint3D(stoi(params[3][0]),
					stoi(params[3][1]),
					stoi(params[3][2]));

				if (params[1].size() > 3) {
					p1.SetSurfaceColor(Color::FromNormalizedRGB(stod(params[1][3]),
						stod(params[1][4]),
						stod(params[1][5])));
					p2.SetSurfaceColor(Color::FromNormalizedRGB(stod(params[2][3]),
						stod(params[2][4]),
						stod(params[2][5])));
					p3.SetSurfaceColor(Color::FromNormalizedRGB(stod(params[3][3]),
						stod(params[3][4]),
						stod(params[3][5])));
				}
				if (hasNormal) {
					D3Vector<double> v1 = D3Vector<double>(normals[0][1], normals[0][2], normals[0][2]);
					D3Vector<double> v2 = D3Vector<double>(normals[1][1], normals[1][2], normals[1][2]);
					D3Vector<double> v3 = D3Vector<double>(normals[2][1], normals[2][2], normals[2][2]);
					p1.SetNormalVector(v1);
					p2.SetNormalVector(v2);
					p3.SetNormalVector(v3);
				}
				else {
					//Normal not supplied
					D3Vector<double> left = D3Vector<double>(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
					D3Vector<double> right = D3Vector<double>(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
					D3Vector<double> avgNomal = left.crossproduct(right);
					avgNomal.Normalize();
					p1.SetNormalVector(avgNomal);
					p2.SetNormalVector(avgNomal);
					p3.SetNormalVector(avgNomal);
				}
				//p1.color = lightParam.GetLightColor(p1);
				//p2.color = lightParam.GetLightColor(p2);
				//p3.color = lightParam.GetLightColor(p3);

				DepthTriangleDrawer3D drawer(&scene, &camera, &lightParam);
				drawer.drawTriangle3D({ p1,p2,p3 }, transMatrix);
				//TriangleDrawer3D drawer(_drawable);
				//ShapeHelper3D::drawTriangle3D(pane, &drawer, { p1,p2,p3 }, transMatrix, colorMode);
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
			else if (words[0] == "camera") {
				Camera cam;
				cam.x_low = stoi(words[1]);
				cam.y_low = stoi(words[2]);
				cam.x_high = stoi(words[3]);
				cam.y_high = stoi(words[4]);
				cam.z_near = -stoi(words[5]);
				cam.z_far = -stoi(words[6]);
				camera = cam;
				scene.SetCamera(cam);
			}
			else if (words[0] == "light") {
				LightSource ls;
				ls.red = stod(words[1]);
				ls.green = stod(words[2]);
				ls.blue = stod(words[3]);
				ls.a = stod(words[4]);
				ls.b = stod(words[5]);
				lightParam.lightSources.push_back(ls);
			}
			else if (words[0] == "surface") {
				auto params = extractLine_Surface(line);
				SurfaceLight sl;
				sl.red = stod(params[1][0]);
				sl.green = stod(params[1][1]);
				sl.blue = stod(params[1][2]);
				sl.ks = stod(params[2][1]);
				sl.alpha = stod(params[2][2]);
				lightParam.surfaceLight = sl;
			}
			else if (words[0] == "ambient") {
				AmbientLight al;
				auto params = extractParameters(line);
				al.red = stod(params[1][0]);
				al.green = stod(params[1][1]);
				al.blue = stod(params[1][2]);
				lightParam.ambientLight = al;
			}
			else if (words[0] == "phong") {
				renderStyle = RenderStyle::Phong;
			}
			else if (words[0] == "gouraud") {
				renderStyle = RenderStyle::Gouraud;
			}
			else if (words[0] == "flat") {
				renderStyle = RenderStyle::Flat;
			}
		}
		scene.render_all();
	}
};

