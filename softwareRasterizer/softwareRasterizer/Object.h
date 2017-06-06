#ifndef OBJECT_H_
#define OBJECT_H_

#include <vector.h>
#include <matrix.h>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct color
{
	int r;
	int g;
	int b;
};

class Object
{
public:
	int textureWidth;
	int textureHeight;

	char *textureRGB;

	Object(int, int);
	~Object();

	Matrix matrix;

	vector<Vector> verticies;
	vector<Vector> textureUV;
	vector<Vector> vertexNormals;
	vector<Vector> faces;

	void loadData(string);//string = folder/filename 
	void loadTexture(LPCWSTR);//string = folder/texture

};

#endif