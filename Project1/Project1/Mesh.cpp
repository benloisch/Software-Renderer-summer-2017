#include "Mesh.h"

void Mesh::loadTexture(string filename) {
	texture = new ImageBMP;
	texture->loadBMP(filename);
}

void Mesh::loadModel(string filename) {
	filename.append(".obj");
	ifstream file(filename);

	string line;

	vector<Vector4D> verticies;
	vector<Vector4D> texCoords;
	vector<Vector4D> normals;
	vector<Face> faces;

	while (file >> line) {
		if (line == "v") {

			float x, y, z = 0.0f;
			file >> x;
			file >> y;
			file >> z;

			verticies.push_back(Vector4D(x, y, z, 1.0f));
		}
		else if (line == "vt") {
			string tx, ty;
			file >> tx;
			file >> ty;
			texCoords.push_back(Vector4D(stof(tx), stof(ty), 0.0f, 0.0f));
		}
		else if (line == "vn") {
			string nx, ny, nz;
			file >> nx;
			file >> ny;
			file >> nz;

			normals.push_back(Vector4D(stof(nx), stof(ny), stof(nz), 0.0f));
		}
		else if (line == "f") {
			for (int i = 0; i < 3; i++) {
				int v = 1, t = 1, n = 1;
				
				string str;
				file >> str;

				int firstNumIndex = 0;
				int secondNumIndex = 0;
				for (unsigned int i = 0; i < str.size(); i++) {
					if (str[i] == '/') {
						firstNumIndex = i;
						break;
					}
				}
				for (unsigned int i = firstNumIndex + 1; i < str.size(); i++) {
					if (str[i] == '/') {
						secondNumIndex = i;
						break;
					}
				}
				
				v = stoi(str.substr(0, firstNumIndex));

				if (texCoords.size() > 0 && secondNumIndex > 0) {
					t = stoi(str.substr(firstNumIndex+1, secondNumIndex));
				}
				else {
					t = stoi(str.substr(firstNumIndex + 1, str.size()));
				}

				if (normals.size() > 0 && firstNumIndex > 0 && secondNumIndex > 0) {
					n = stoi(str.substr(secondNumIndex+1, str.size()));
				}

				faces.push_back(Face(v, t, n));
			}
		}
	}

	if (normals.size() == 0)
		normals.push_back(Vector4D(0, 0, 0, 0));

	//now load the verticies from the faces
	for (int i = 0; i < faces.size(); i++) {
		Vector4D tex = texCoords[faces[i].textureIndex - 1];
		if (tex.x < 0)
			tex.x = -tex.x;
		if (tex.y < 0)
			tex.y = -tex.y;

		if (tex.y > 1)
			tex.y = tex.y - floorf(tex.y);
		if (tex.x > 1)
			tex.x = tex.x - floorf(tex.x);

		this->verticies.push_back(Vertex(verticies[faces[i].vertexIndex - 1], tex, normals[faces[i].normalIndex - 1]));
	}

}