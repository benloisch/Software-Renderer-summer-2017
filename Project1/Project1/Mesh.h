#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <string>
#include <iostream>

#include "Vertex.h"
#include "Vector4D.h"
#include "ImageBMP.h"

using namespace std;

extern int trianglesLoaded;

class Face {
public:

	int vertexIndex;
	int textureIndex;
	int normalIndex;

	Face::Face() : vertexIndex(1), textureIndex(1), normalIndex(1) {};
	Face::Face(int v, int t, int n) : vertexIndex(v), textureIndex(t), normalIndex(n) {};
};

class Mesh {

public:

	bool zCull;

	Mesh::Mesh() {
		zCull = true;
	}

	Matrix4x4 modelMesh;
	vector<Vertex> verticies;
	ImageBMP texture;

	void loadModel(string filename);
	void loadTexture(string filename);

private:



};




#endif