#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <rapidobj/rapidobj.hpp>
#include "../support/error.hpp"

Mesh::Mesh()
	:mLoaded(false)
{

}

Mesh::~Mesh() 
{
	//Destructor Method
	glDeleteVertexArrays(1, &mvao);
	glDeleteBuffers(1, &mvbo);
	glDeleteBuffers(1, &mvbocol);
	glDeleteBuffers(1, &mvbotex);
	glDeleteBuffers(1, &mvbonormal);
}

bool Mesh::loadOBJ(char const* aPath)
{
	//return {};
		// Ask rapidobj to load the requested file
	rapidobj::MaterialLibrary mtllib = rapidobj::MaterialLibrary::Default(rapidobj::Load::Optional);
	auto result = rapidobj::ParseFile(aPath, mtllib);
	if (result.error)
		throw Error("Unable to load OBJ file ’%s’: %s", aPath, result.error.code.message().c_str());
	// OBJ files can define faces that are not triangles. However, OpenGL will only render triangles (and lines
	// and points), so we must triangulate any faces that are not already triangles. Fortunately, rapidobj can do
	// this for us.
	rapidobj::Triangulate(result);

	// Convert the OBJ data into a SimpleMeshData structure. For now, we simply turn the object into a triangle
	// soup, ignoring the indexing information that the OBJ file contains.

	for (auto const& shape : result.shapes) {
		for (std::size_t i = 0; i < shape.mesh.indices.size(); ++i) {
			auto const& idx = shape.mesh.indices[i];

			positions.emplace_back(Vec3f{
				result.attributes.positions[idx.position_index * 3 + 0],
				result.attributes.positions[idx.position_index * 3 + 1],
				result.attributes.positions[idx.position_index * 3 + 2]
				});
			textures.emplace_back(Vec2f{
				result.attributes.texcoords[idx.texcoord_index * 2 + 0],
				result.attributes.texcoords[idx.texcoord_index * 2 + 1]
				});
			normals.emplace_back(Vec3f{
				result.attributes.normals[idx.normal_index * 3 + 0],
				result.attributes.normals[idx.normal_index * 3 + 1],
				result.attributes.normals[idx.normal_index * 3 + 2]
				});

			// Always triangles, so we can find the face index by dividing the vertex index by three
			auto const& mat = result.materials[shape.mesh.material_ids[i / 3]];
			// Just replicate the material ambient color for each vertex...
			colors.emplace_back(Vec3f{
				mat.ambient[0],
				mat.ambient[1],
				mat.ambient[2]
				});
		}
	}
	initBuffers();
	mLoaded = true;

	return true;
}
void Mesh::draw()
{
	if (!mLoaded) return;

	glBindVertexArray(mvao);
	glDrawArrays(GL_TRIANGLES, 0, positions.size());
	glBindVertexArray(0);
}

void Mesh::initBuffers() {
	//Position 
	glGenBuffers(1, &mvbo);
	glBindBuffer(GL_ARRAY_BUFFER, mvbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3f), positions.data(), GL_STATIC_DRAW);
	//Color
	glGenBuffers(1, &mvbocol);
	glBindBuffer(GL_ARRAY_BUFFER, mvbocol);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vec3f), colors.data(), GL_STATIC_DRAW);
	//Texture
	glGenBuffers(1, &mvbotex);
	glBindBuffer(GL_ARRAY_BUFFER, mvbotex);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(Vec2f), textures.data(), GL_STATIC_DRAW);
	//Normal
	glGenBuffers(1, &mvbonormal);
	glBindBuffer(GL_ARRAY_BUFFER, mvbonormal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3f), normals.data(), GL_STATIC_DRAW);


	glGenVertexArrays(1, &mvao); //create vao
	glBindVertexArray(mvao); //bind vao

	// Position attribute
	glBindBuffer(GL_ARRAY_BUFFER, mvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//Color
	glBindBuffer(GL_ARRAY_BUFFER, mvbocol);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//Texture
	glBindBuffer(GL_ARRAY_BUFFER, mvbotex);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//Normal
	glBindBuffer(GL_ARRAY_BUFFER, mvbonormal);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}