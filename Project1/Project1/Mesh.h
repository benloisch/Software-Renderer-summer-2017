#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include "Vertex.h"
#include "Vector4D.h"
#include "ImageBMP.h"

using namespace std;

class Mesh {

public:

	Matrix4x4 modelMesh;
	vector<Vertex> verticies;
	ImageBMP *texture;

private:



};




#endif