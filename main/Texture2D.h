#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad.h>
#include <string>

using std::string;
class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const string& filename, bool genMipMaps = true);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);

private:
	GLuint mTexture;
};

#endif //TEXTURE2D_H