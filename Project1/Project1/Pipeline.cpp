#include "Pipeline.h"

Pipeline::Pipeline() {
	buffer = NULL;
	cam = NULL;
}

void Pipeline::transform(Mesh &inputMesh) {

	MVP = inputMesh.modelMesh * cam->viewMatrix.inverse() * cam->projectionMatrix;

	for (unsigned int v = 0; v < inputMesh.verticies.size(); v += 3) {

		//perform z-culling
		if (((inputMesh.verticies[v + 1].v - inputMesh.verticies[v].v) ^
			(inputMesh.verticies[v + 2].v - inputMesh.verticies[v].v)).z > 0) {
			//continue;
		}

		verticies[0] = inputMesh.verticies[v];
		verticies[1] = inputMesh.verticies[v+1];
		verticies[2] = inputMesh.verticies[v+2];

		/*
		texCoords[0] = inputMesh.texCoords[v];
		texCoords[1] = inputMesh.texCoords[v+1];
		texCoords[2] = inputMesh.texCoords[v+2];

		normals[0] = inputMesh.normals[v];
		normals[1] = inputMesh.normals[v+1];
		normals[2] = inputMesh.normals[v+2];
		*/

		projectVerticies();
		clipVerticies();
		shadeTriangle();
	}
}