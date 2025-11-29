#include "ComputeShader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"

ComputeShader::ComputeShader(const std::string& filepath)
    : m_FilePath(filepath), m_ComputeShaderID(0)
{
    ComputeShaderProgramSource source = ParseShader(filepath);
    m_ComputeShaderID = CreateShader(source.ComputeSource);
}

ComputeShader::~ComputeShader()
{
    GLCall(glDeleteProgram(m_ComputeShaderID));
}

void ComputeShader::Bind() const
{
    GLCall(glUseProgram(m_ComputeShaderID));
}

void ComputeShader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void ComputeShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void ComputeShader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int ComputeShader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_ComputeShaderID, name.c_str()));
    if (location == -1)
        std::cout << "warning: uniform " << name << " doesn't exist !" << std::endl;
    m_UniformLocationCache[name] = location;
    return location;
}

unsigned int ComputeShader::CreateShader(const std::string& source)
{
    unsigned int program = glCreateProgram();
    unsigned int cs = CompileShader(GL_COMPUTE_SHADER, source);

    glAttachShader(program, cs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(cs);

    return program;
}

unsigned int ComputeShader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile compute shader !" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }
    return id;
}


ComputeShaderProgramSource ComputeShader::ParseShader(const std::string& filepath) {
    std::fstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[1]; // Correct way to declare array of stringstream

    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
            ss[0] << line << '\n';

    }
    return { ss[0].str()};
}

