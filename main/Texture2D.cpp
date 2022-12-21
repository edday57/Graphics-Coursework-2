#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
Texture2D::Texture2D()
	: mTexture(0)
{
}
Texture2D::~Texture2D()
{
	glDeleteTextures(1, &mTexture);
}

bool Texture2D::loadTexture(const string& filename, bool genMipMaps) {
	
	int width, height, components;
	
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (imageData == NULL) {
		std::cerr << "Error loading texture " << filename << std::endl;
		return false;
	}

	//Invert image
	int widthBytes = width * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;
	for (int row = 0; row < halfHeight; row++) {
		top = imageData + row * widthBytes;
		bottom = imageData + (height - row-1) * widthBytes;
		for (int col = 0; col < widthBytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	//configure texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//map bits to opengl texture
	//opengl texture, mipmap level of detail, texture type(RGBA), width, height, border
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	if (genMipMaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	//clear memory and unbind texture
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
void Texture2D::bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture2D::unbind(GLuint texUnit) 
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}