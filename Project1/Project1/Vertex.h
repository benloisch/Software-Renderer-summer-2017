#ifndef VERTEX_H_
#define VERTEX_H_

#include "Vector4D.h"

class Vertex {
public:
	Vector4D v;

	Vertex() : v(0, 0, 0, 0) {}
	Vertex(double x, double y, double z, double w) : v(x, y, z, w) {}

	Vertex(const Vertex &vertex);
	Vertex(const Vector4D &vector);
	Vertex& operator=(const Vertex &vertex);
	Vertex& operator=(const Vector4D &vector);

private:


};

inline
Vertex& Vertex::operator=(const Vertex &vertex)
{
	this->v.x = vertex.v.x;
	this->v.y = vertex.v.y;
	this->v.z = vertex.v.z;

	return *this;
}

inline
Vertex& Vertex::operator=(const Vector4D &vector)
{
	this->v.x = vector.x;
	this->v.y = vector.y;
	this->v.z = vector.z;

	return *this;
}

#endif