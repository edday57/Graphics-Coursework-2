#ifndef SHADER_PROGRAM2_H
#define SHADER_PROGRAM2_H

#include <glad.h>
#include <string>
using std::string;
#include<map>
#include "../vmlib/vec2.hpp"
#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"
class ShaderProgram2
{
public:
	ShaderProgram2();
	~ShaderProgram2();

	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsfilename, const char* fsFilename);
	void use();
	void setUniform(const GLchar* name, const Vec2f v);
	void setUniform(const GLchar* name, const Vec3f v);
	void setUniform(const GLchar* name, const Vec4f v);
	void setUniform(const GLchar* name, const Mat44f m);

	GLuint getProgram()const;

private:
	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLuint mHandle;

	GLint getUniformLocation(const GLchar* name);

	std::map<string, GLint> mUniformLocations;
};

#endif // SHADER_PROGRAM2_H