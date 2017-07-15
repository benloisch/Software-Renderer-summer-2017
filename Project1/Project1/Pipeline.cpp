#include "Pipeline.h"

Pipeline::Pipeline() {
	buffer = NULL;
	cam = NULL;
	mesh = NULL;

	for (int i = 0; i < 8; i++)
		verticies[i] = Vertex(Vector4D(0, 0, 0, 0), Vector4D(0, 0, 0, 0), Vector4D(0, 0, 0, 0));
}

void Pipeline::transform(Mesh *inputMesh) {

	mesh = inputMesh;

	Matrix4x4 MVP = mesh->modelMesh * cam->viewMatrix.inverse() * cam->projectionMatrix;

	for (unsigned int v = 0; v < mesh->verticies.size(); v += 3) {

		//Vertex verticies[3];

		verticies[0] = mesh->verticies[v];
		verticies[1] = mesh->verticies[v + 1];
		verticies[2] = mesh->verticies[v + 2];

		projectVerticies(MVP);
		
		int verticiesSize = clipVerticies();

		//clip and add extra verticies in clockwise order
		if (verticiesSize >= 2) {
			for (unsigned int i = 0; i < verticiesSize - 2; i++)
				shadeTriangle(verticies[0], verticies[i + 1], verticies[i + 2]);
		}
	}
}