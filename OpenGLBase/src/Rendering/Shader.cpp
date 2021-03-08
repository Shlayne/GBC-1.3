#include "Shader.h"
#include <gl/glew.h>
#include <iostream> // TODO: logging
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(const std::vector<ShaderFile>& shaders)
{
	rendererID = glCreateProgram();
	int shaderCount = (int)shaders.size();
	unsigned int* shaderIDs = (unsigned int*)alloca(shaderCount * sizeof(unsigned int));
	for (int i = 0; i < shaderCount; i++)
	{
		shaderIDs[i] = Compile(shaders[i]);
		glAttachShader(rendererID, shaderIDs[i]);
	}
	
	bool success = LinkAndValidate();
	
	// Do this regardles of success
	for (int i = 0; i < shaderCount; i++)
	{
		glDetachShader(rendererID, shaderIDs[i]);
		glDeleteShader(shaderIDs[i]);
	}

	if (!success)
	{
		glDeleteProgram(rendererID);
		rendererID = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(rendererID);
}

void ShaderProgram::Bind() const
{
	glUseProgram(rendererID);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

bool ShaderProgram::LinkAndValidate()
{
	// Link
	glLinkProgram(rendererID);
	int link;
	glGetProgramiv(rendererID, GL_LINK_STATUS, &link);
	if (link == GL_FALSE)
	{
		int length;
		glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(rendererID, length, &length, message);
		std::cerr << "Failed to link program:\n" << message << '\n';
		return false;
	}

	// Validate
	glValidateProgram(rendererID);
	int validate;
	glGetProgramiv(rendererID, GL_VALIDATE_STATUS, &validate);
	if (validate == GL_FALSE)
	{
		int length;
		glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(rendererID, length, &length, message);
		std::cerr << "Failed to validate program:\n" << message << '\n';
		return false;
	}

	return true;
}

static GLuint GetGLType(ShaderType type)
{
	switch (type)
	{
		case ShaderType::Vertex:                return GL_VERTEX_SHADER;
		case ShaderType::TessolationControl:    return GL_TESS_CONTROL_SHADER;
		case ShaderType::TessolationEvaluation: return GL_TESS_EVALUATION_SHADER;
		case ShaderType::Geometry:              return GL_GEOMETRY_SHADER;
		case ShaderType::Fragment:              return GL_FRAGMENT_SHADER;
		case ShaderType::Compute:               return GL_COMPUTE_SHADER;
		default:                                return 0;
	}
}

static const char* GetName(ShaderType type)
{
	switch (type)
	{
		case ShaderType::Vertex:                return "vertex";
		case ShaderType::TessolationControl:    return "tessolation control";
		case ShaderType::TessolationEvaluation: return "tessolation evaluation";
		case ShaderType::Geometry:              return "geometry";
		case ShaderType::Fragment:              return "fragment";
		case ShaderType::Compute:               return "compute";
		default:                                return "unknown";
	}
}

unsigned int ShaderProgram::Compile(const ShaderFile& shader)
{
	unsigned int id = glCreateShader(GetGLType(shader.type));
	const char* source = shader.source.c_str();
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int compile;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compile);
	if (compile == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cerr << "Failed to compile " << GetName(shader.type) << " shader:\n" << message << '\n';
		glDeleteShader(id);
		return 0;
	}

	return id;
}

int ShaderProgram::GetUniformLocation(const std::string& name) const
{
	auto it = uniformLocations.find(name);
	if (it != uniformLocations.end())
		return it->second;

	int location = glGetUniformLocation(rendererID, name.c_str());
	uniformLocations[name] = location;
	return location;
}

template<> void ShaderProgram::SetUniform<float>(const std::string& name, const float& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1fv(location, 1, &value);
}
template<> void ShaderProgram::SetUniform<glm::vec2>(const std::string& name, const glm::vec2& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform2fv(location, 1, glm::value_ptr(value));
}
template<> void ShaderProgram::SetUniform<glm::vec3>(const std::string& name, const glm::vec3& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform3fv(location, 1, glm::value_ptr(value));
}
template<> void ShaderProgram::SetUniform<glm::vec4>(const std::string& name, const glm::vec4& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform4fv(location, 1, glm::value_ptr(value));
}

template<> void ShaderProgram::SetUniform<int>(const std::string& name, const int& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1iv(location, 1, &value);
}
template<> void ShaderProgram::SetUniform<unsigned int>(const std::string& name, const unsigned int& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1uiv(location, 1, &value);
}

template<> void ShaderProgram::SetUniform<glm::mat4>(const std::string& name, const glm::mat4x4& value) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}


template<> void ShaderProgram::SetUniforms<int>(const std::string& name, const int* values, int count) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1iv(location, count, values);
}

template<> void ShaderProgram::SetUniforms<unsigned int>(const std::string& name, const unsigned int* values, int count) const
{
	int location = GetUniformLocation(name);
	if (location != -1) glUniform1uiv(location, count, values);
}