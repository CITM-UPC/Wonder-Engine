#pragma once
#include "glm/glm.hpp"

#include "types.h"
#include "Component.h"
#include "TransformComp.h"
#include "MeshComp.h"
#include "TextureComp.h"
#include "Graphic.h"


#include <string>
#include <sstream>
#include <vector>

enum Shape {
	CUBE = 0,
	SPHERE,
	CYLINDER,
	PLANE
};

struct Cube : Graphic
{
	static const int NUM_FACES = 6;
	static const int NUM_TRIANGLES = NUM_FACES * 2;
	static const int NUM_VERTEXS = NUM_TRIANGLES * 3;
	static int side_length;

	const vec3 a;
	const vec3 b;
	const vec3 c;
	const vec3 d;
	const vec3 e;
	const vec3 f;
	const vec3 g;
	const vec3 h;

	//const vec3 red;
	//const vec3 green;
	//const vec3 blue;
	//const vec3 yellow;
	//const vec3 white;
	//const vec3 black;

	Cube();
};

struct Sphere : Graphic
{
	static int radius;
	static int num_subdivisions;
	std::vector<float> vertices;

	Sphere();
};

struct Cylinder : Graphic
{
	static int height;
	static int radius;

	Cylinder();
};

struct Plane : Graphic
{
	static const int NUM_FACES = 1;
	static const int NUM_TRIANGLES = NUM_FACES * 2;
	static const int NUM_VERTEXS = NUM_TRIANGLES * 3;
	static int side_length;

	Plane();
};
