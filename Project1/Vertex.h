#ifndef VERTEX_H_
#define VERTEX_H_

#include "Vector4D.h"

class Vertex {
public:
	Vector4D v;
	Vector4D t;
	Vector4D n;

	Vertex() : v(0, 0, 0, 0), t(0, 0, 0, 0), n(0, 0, 0, 0) {}
	Vertex(Vector4D &vin, Vector4D &tin, Vector4D &nin);

	inline Vertex& operator=(const Vertex &vertex);

private:


};

inline
Vertex& Vertex::operator=(const Vertex &vertex)
{
	this->v.x = vertex.v.x;
	this->v.y = vertex.v.y;
	this->v.z = vertex.v.z;
	this->v.w = vertex.v.w;

	this->t.x = vertex.t.x;
	this->t.y = vertex.t.y;

	this->n.x = vertex.n.x;
	this->n.y = vertex.n.y;
	this->n.z = vertex.n.z;

	return *this;
}

#endif