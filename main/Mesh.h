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
	Vec3f normal;
	Vec2f texCoords;
};
class Mesh
{
public:
		Mesh();
		~Mesh();

		bool loadOBJ(char const* aPath);
		void draw();

private:
	void initBuffers();

	bool mLoaded;
	std::vector<Vec3f> positions;
	std::vector<Vec3f> colors;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> textures;
	GLuint mvbo, mvbocol, mvbotex, mvbonormal, mvao;

};

#endif