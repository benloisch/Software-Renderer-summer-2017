#include "Pipeline.h"

Pipeline::Pipeline() {
	buffer = NULL;
	cam = NULL;
	mesh = NULL;
}

void Pipeline::transform(Mesh *inputMesh) {

	mesh = inputMesh;

	Matrix4x4 MVP = mesh->modelMesh * cam->viewMatrix.inverse() * cam->projectionMatrix;

	for (unsigned int v = 0; v < mesh->verticies.size(); v += 3) {

		vector<Vertex> verticies(3);

		verticies[0] = mesh->verticies[v];
		verticies[1] = mesh->verticies[v + 1];
		verticies[2] = mesh->verticies[v + 2];

		projectVerticies(&verticies[0], MVP);
		
		//clip and add extra verticies in clockwise order
		vector<Vertex> copy = clipVerticies(verticies);
		
		if (copy.size() > 0)
			verticies = copy;

		for (unsigned int i = 0; i < verticies.size() - 2; i++)
			shadeTriangle(&verticies[i], &verticies[i].t, &verticies[i].n);
	}
}