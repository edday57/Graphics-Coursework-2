#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>

Mesh::Mesh()
	:mLoaded(false)
{

}

Mesh::~Mesh() 
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
}

bool Mesh::loadOBJ(const std::string& filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices;
	std::vector<Vec3f> tempVertices;
	std::vector<Vec2f> tempUVs;

	if (filename.find(".obj") != std::string::npos)
	{
		std::ifstream fin(filename, std::ios::in);
		if (!fin)
		{
			std::cerr << "Cannot open " << filename << std::endl;
			return false;
 		}
		std::cout << "Loading OBJ file " << filename << "..." << std::endl;
		std::string lineBuffer;
		while (std::getline(fin, lineBuffer))
		{
			if (lineBuffer.substr(0, 2) == "v ") {
				std::istringstream v(lineBuffer.substr(2));
				Vec3f vertex;
				v >> vertex.x; v >> vertex.y; v >> vertex.z;
				tempVertices.push_back(vertex);
			}
			else if (lineBuffer.substr(0, 2) == "vt") {
				std::istringstream vt(lineBuffer.substr(3));
				Vec2f uv;
				vt >> uv.x; vt >> uv.y;
				tempUVs.push_back(uv);
			}
			else if (lineBuffer.substr(0, 2) == "f ") 
			{
				int p1, p2, p3;
				int t1, t2, t3;
				int n1, n2, n3;
				const char* face = lineBuffer.c_str();
				int match = sscanf_s(face, "f %i/%i/%i %i/%i/%i %i/%i/%i",
					&p1, &t1, &n1,
					&p2, &t2, &n2,
					&p3, &t3, &n3);
				if (match != 9) 
					std::cout << "Failed to parse OBJ file" << std::endl;

				vertexIndices.push_back(p1);
				vertexIndices.push_back(p2);
				vertexIndices.push_back(p3);

				uvIndices.push_back(t1);
				uvIndices.push_back(t2);
				uvIndices.push_back(t3);
				}
			}
			fin.close();

			for (unsigned int i = 0; i < vertexIndices.size(); i++)
			{
				Vec3f vertex = tempVertices[vertexIndices[i] - 1];
				Vec2f uv = tempUVs[uvIndices[i] - 1];

				Vertex meshVertex; 
				meshVertex.position = vertex;
				meshVertex.texCoords = uv;
				mVertices.push_back(meshVertex);
			}
			initBuffers();
			return (mLoaded = true);
		}
	
	return false;
}
void Mesh::draw()
{
	if (!mLoaded) return;

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
	glBindVertexArray(0);
}

void Mesh::initBuffers() {
	//Position and tex
	glGenBuffers(1, &mVBO); //create buffer
	glBindBuffer(GL_ARRAY_BUFFER, mVBO); // bind buffer
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW); //Move to GPU


	//glGenBuffers(1, &vbo2); //create buffer
	//glBindBuffer(GL_ARRAY_BUFFER, vbo2); // bind buffer
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color), vertex_color, GL_STATIC_DRAW); //Move to GPU

	glGenVertexArrays(1, &mVAO); //create vao
	glBindVertexArray(mVAO); //bind vao

	// Position attribute
	//glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);



	// Texture Coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat))); //5 stride, 3 float offset
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}