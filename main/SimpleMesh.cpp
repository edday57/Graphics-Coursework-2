#include "SimpleMesh.h"
#include <rapidobj/rapidobj.hpp>

#include "../support/error.hpp"

SimpleMeshData concatenate(SimpleMeshData aM, SimpleMeshData const& aN)
{
	aM.positions.insert(aM.positions.end(), aN.positions.begin(), aN.positions.end());
	aM.colors.insert(aM.colors.end(), aN.colors.begin(), aN.colors.end());
	return aM;
}


GLuint create_vao(SimpleMeshData aMeshData)
{

	glGenBuffers(1, &(aMeshData.mvbo));
	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbo);
	glBufferData(GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof(Vec3f), aMeshData.positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &(aMeshData.mvbocol));
	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbocol);
	glBufferData(GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof(Vec3f), aMeshData.colors.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &(aMeshData.mvbonormal));
	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbonormal);
	glBufferData(GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof(Vec3f), aMeshData.normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &(aMeshData.mvbotex));
	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbotex);
	glBufferData(GL_ARRAY_BUFFER, aMeshData.textures.size() * sizeof(Vec2f), aMeshData.textures.data(), GL_STATIC_DRAW);

	// VAO

	glGenVertexArrays(1, &(aMeshData.mvao));
	glBindVertexArray(aMeshData.mvao);

	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbo);

	glVertexAttribPointer(
		0, // location = 0 in vertex shader
		3, GL_FLOAT, GL_FALSE, // 2 floats, not normalized to [0..1] (GL FALSE)
		0, // stride = 0 indicates that there is no padding between inputs
		0 // data starts at offset 0 in the VBO.
	);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbocol);
	glVertexAttribPointer(
		1, // location = 1 in vertex shader
		3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE)
		0, // see above
		0 // see above
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbotex);
	glVertexAttribPointer(
		2, // location = 1 in vertex shader
		2, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE)
		0, // see above
		0 // see above
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, aMeshData.mvbonormal);
	glVertexAttribPointer(
		3, // location = 1 in vertex shader
		3, GL_FLOAT, GL_FALSE, // 3 floats, not normalized to [0..1] (GL FALSE)
		0, // see above
		0 // see above
	);
	glEnableVertexAttribArray(3);

	// Reset state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Clean up buffers
	// Note: these are not deleted fully, as the VAO holds a reference to them.
	glDeleteBuffers(1, &aMeshData.mvbo);
	glDeleteBuffers(1, &aMeshData.mvbocol);
	glDeleteBuffers(1, &aMeshData.mvbotex);

	return(aMeshData.mvao);
}

SimpleMeshData load_wavefront_obj(char const* aPath)
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
	SimpleMeshData ret;

	for (auto const& shape : result.shapes) {
		for (std::size_t i = 0; i < shape.mesh.indices.size(); ++i) {
			auto const& idx = shape.mesh.indices[i];

			ret.positions.emplace_back(Vec3f{
				result.attributes.positions[idx.position_index * 3 + 0],
				result.attributes.positions[idx.position_index * 3 + 1],
				result.attributes.positions[idx.position_index * 3 + 2]
				});
			ret.textures.emplace_back(Vec2f{
				result.attributes.texcoords[idx.texcoord_index * 2 + 0],
				result.attributes.texcoords[idx.texcoord_index * 2 + 1]
				});
			ret.normals.emplace_back(normalize(Vec3f{
				result.attributes.normals[idx.normal_index * 2 + 0],
				result.attributes.normals[idx.normal_index * 2 + 1],
				result.attributes.normals[idx.normal_index * 2 + 2]
				}));

			// Always triangles, so we can find the face index by dividing the vertex index by three
			auto const& mat = result.materials[shape.mesh.material_ids[i / 3]];
			// Just replicate the material ambient color for each vertex...
			ret.colors.emplace_back(Vec3f{
				mat.ambient[0],
				mat.ambient[1],
				mat.ambient[2]
				});
		}
	}
	return ret;
}