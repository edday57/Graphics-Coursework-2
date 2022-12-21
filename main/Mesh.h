#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include "glad.h"
#include "../vmlib/mat44.hpp"
#include "../vmlib/vec2.hpp"

struct Vertex
{
	Vec3f position;
	Vec2f texCoords;
};
class Mesh
{
public:
		Mesh();
		~Mesh();

		bool loadOBJ(const std::string& filename);
		void draw();

private:
	void initBuffers();

	bool mLoaded;
	std::vector<Vertex> mVertices;

	GLuint  mVBO, mVAO;

};

#endif