#include "OpenGLUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OPenGL Error] (" << error << "): " << function << " in " << file << " line: " << line << std::endl;
        return false;
    }
    return true;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int CreateComputeShader(const std::string& computeShader)
{
    unsigned int program = glCreateProgram();
    unsigned int cs = CompileShader(GL_COMPUTE_SHADER, computeShader);
    GLCall(glAttachShader(program, cs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    GLCall(glDeleteShader(cs));

    return program;
}


unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        // COMPUTE-SHADER FEHLERMELDUNG HINZUFÜGEN
        const char* shaderType =
            (type == GL_VERTEX_SHADER) ? "vertex" :
            (type == GL_FRAGMENT_SHADER) ? "fragment" :
            (type == GL_COMPUTE_SHADER) ? "compute" : "unknown";

        std::cout << "failed to compile " << shaderType << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

ShaderProgramSource ParseShader(const std::string& filepath) {
    std::fstream stream(filepath);

    enum class ShaderType
    {
		NONE = -1, VERTEX = 0, FRAGMENT = 1, COMPUTE = 2
    };

    std::string line;
    std::stringstream ss[3]; // Correct way to declare array of stringstream

    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("compute") != std::string::npos)
				type = ShaderType::COMPUTE;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return{ ss[0].str(), ss[1].str(), ss[2].str() };
};