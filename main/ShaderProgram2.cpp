#include "ShaderProgram2.h"
#include <fstream>
#include <iostream>
#include <sstream>

ShaderProgram2::ShaderProgram2()
	: mHandle(0)
{
	
}
ShaderProgram2::~ShaderProgram2() 
{
	glDeleteProgram(mHandle);
}

bool ShaderProgram2::loadShaders(const char* vsfilename, const char* fsFilename) {
	
	string vsString = fileToString(vsfilename);
	string fsString = fileToString(fsFilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	glCompileShader(vs);
	checkCompileErrors(vs, VERTEX);

	glCompileShader(fs);
	checkCompileErrors(fs, FRAGMENT);


	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);
	glLinkProgram(mHandle);

	checkCompileErrors(mHandle, PROGRAM);
	
	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();

	return true;
}
void ShaderProgram2::use() {
	if (mHandle > 0) {
		glUseProgram(mHandle);
	}
}

string ShaderProgram2::fileToString(const string& filename) 
{
	std::stringstream ss;
	std::ifstream file;

	try {
		file.open(filename, std::ios::in);
		if (!file.fail()) {
			ss << file.rdbuf();
		}
		file.close();
	}
	catch(std::exception ex) {
		std::cerr << "Error reading filename" << std::endl;
	}
	return ss.str();
	
}
void ShaderProgram2::checkCompileErrors(GLuint shader, ShaderType type) 
{
	int status = 0;
	if (type == PROGRAM) {
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "Error! Program failed to link. " << errorLog << std::endl;
		}
	}
	else { //VERTEX or FRAGMENT
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
		}
	}
}
GLuint ShaderProgram2::getProgram()const {
	return mHandle;
}

GLint ShaderProgram2::getUniformLocation(const GLchar* name) {
	std::map<std::string, GLint>::iterator it = mUniformLocations.find(name);
	if (it == mUniformLocations.end()) {
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}
	return mUniformLocations[name];
}

void ShaderProgram2::setUniform(const GLchar* name, const Vec2f v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}
void ShaderProgram2::setUniform(const GLchar* name, const Vec3f v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}
void ShaderProgram2::setUniform(const GLchar* name, const Vec4f v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram2::setUniform(const GLchar* name, const Mat44f m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_TRUE, &m(0,0));
}

void ShaderProgram2::setUniform(const GLchar* name, const GLfloat& f)
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, f);
}
void ShaderProgram2::setUniformSampler(const GLchar* name, const GLint& slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, slot);
}