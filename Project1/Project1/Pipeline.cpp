#include "Pipeline.h"

Pipeline::Pipeline() {
	buffer = NULL;
	cam = NULL;
	//mesh = NULL;

	trianglesRendered = 0;

	for (int i = 0; i < 14; i++)
		verticies[i] = Vertex(Vector4D(0, 0, 0, 0), Vector4D(0, 0, 0, 0), Vector4D(0, 0, 0, 0));
}

void Pipeline::transform(Mesh *inputMesh) {

	mesh = *inputMesh;

	Matrix4x4 MVP = mesh.modelMesh * cam->viewMatrix.inverse() * cam->projectionMatrix;
	//Matrix4x4 MV = mesh->modelMesh * cam->viewMatrix.inverse();
	//Matrix4x4 V = cam->viewMatrix.inverse();

	for (unsigned int v = 0; v < mesh.verticies.size(); v += 3) {

		textureWidth = mesh.texture.width;
		textureWidthMinusOne = mesh.texture.width - 1;
		textureHeightMinusOne = mesh.texture.height - 1;

		//Vertex verticies[3];

		verticies[0] = mesh.verticies[v];
		verticies[1] = mesh.verticies[v + 1];
		verticies[2] = mesh.verticies[v + 2];

		projectVerticies(MVP);

		//directionalLight = Vector4D(0.0f, 1.0f, 0.0f, 0.0f) * V;
		//directionalLight.normalize();

		//Cz = AxBy - AyBx
		//perform z-culling
		if (mesh.zCull) {
			if ((((verticies[1].v.x / verticies[1].v.w) - (verticies[0].v.x / verticies[0].v.w))*((verticies[2].v.y / verticies[2].v.w) - (verticies[0].v.y / verticies[0].v.w))) -
				(((verticies[1].v.y / verticies[1].v.w) - (verticies[0].v.y / verticies[0].v.w))*((verticies[2].v.x / verticies[2].v.w) - (verticies[0].v.x / verticies[0].v.w))) > 0) {
				continue;
			}
		}
		
		int verticiesSize = clipVerticies();

		//clip and add extra verticies in clockwise order
		if (verticiesSize >= 2) {

			//verticies[0].n *= MV;
			//verticies[0].n.normalize();
			//verticies[1].n *= MV;
			//verticies[1].n.normalize();

			for (int i = 0; i < verticiesSize - 2; i++) {
				//thread t(&Pipeline::shadeTriangle, this, verticies[0], verticies[i + 1], verticies[i + 2]);
				//t.join();

				//verticies[i+2].n *= MV;
				//verticies[i+2].n.normalize();

				shadeTriangle(verticies[0], verticies[i + 1], verticies[i + 2]);
				trianglesRendered++;
			}
		}
	}
}