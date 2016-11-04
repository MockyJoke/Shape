#pragma once
#include <string>
#include <stack>
#include <fstream>
#include "stringhelper.h"
#include <cmath>
#ifdef _WINDOWS
#include "windows.h"
#endif // _WINDOWS
using namespace std;

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
		return m;
	}
	static Matrix GetScaleMatrix(double sx,double sy, double sz) {
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
		degree = degree / 3.141592653;
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
	void PrintMatrix() {
#ifdef _WINDOWS
		OutputDebugString(L"-------------------\r\n");
		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Cols; j++) {
				char buf[32];
				sprintf_s(buf, 32, "%f, ", data[i][j]);
				wstring str(buf,buf+32);
				OutputDebugString(str.c_str());
			}
			OutputDebugString(L"\r\n");
		}
#endif // _WINDOWS

	}
};



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

	void Run() {
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
				string param = line.substr(line.find_first_of(' '), line.length() - 7);
				param.erase(remove_if(param.begin(), param.end(), [](char x) {
					return std::isspace(x) || x == '(';
				}), param.end());
				param = replaceString(param, ")", ",");
				vector<string> paramList = split(param, ',');

			}
			transMatrix.PrintMatrix();
		}
	}
};