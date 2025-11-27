#pragma once
#include <GL/glew.h>
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__,__LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string ComputeSource;
};

int GetUniformLocation(const std::string & name);
unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);
unsigned int CreateComputeShader(const std::string& computeShader);
unsigned int CompileShader(unsigned int type, const std::string & source);
ShaderProgramSource ParseShader(const std::string & filepath);