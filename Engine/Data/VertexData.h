#ifndef _VERTEXDATA_H_
#define _VERTEXDATA_H_

class Vertex 
{
public:
	Vertex():x(0.0f),y(0.0f),z(0.0f)
	{
	}
	Vertex(float x,float y,float z):x(x),y(y),z(z)
	{
	}
	float x,y,z;
};

class VertexData
{
public:
	Vertex position;
	Vertex texture;
	Vertex normal;
};

#endif