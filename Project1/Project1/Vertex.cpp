#include "Vertex.h"

Vertex::Vertex(const Vertex &vertex)
{
	this->v.x = vertex.v.x;
	this->v.y = vertex.v.y;
	this->v.z = vertex.v.z;
	this->v.w = vertex.v.w;
}

Vertex::Vertex(const Vector4D &vector)
{
	this->v.x = vector.x;
	this->v.y = vector.y;
	this->v.z = vector.z;
	this->v.w = vector.w;
}