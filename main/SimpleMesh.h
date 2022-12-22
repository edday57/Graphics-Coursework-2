#ifndef SIMPLE_MESH_H
#define SIMPLE_MESH_H

#include <glad.h>

#include <vector>

#include "../vmlib/vec3.hpp"
#include "../vmlib/vec2.hpp"

struct SimpleMeshData
{
	std::vector<Vec3f> positions;
	std::vector<Vec3f> colors;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> textures;
	GLuint mvbo, mvbocol, mvbotex, mvbonormal, mvao;
};

SimpleMeshData concatenate(SimpleMeshData, SimpleMeshData const&);


GLuint create_vao(SimpleMeshData aMeshData);
SimpleMeshData load_wavefront_obj(char const* aPath);

#endif // SIMPLE_MESH_H
