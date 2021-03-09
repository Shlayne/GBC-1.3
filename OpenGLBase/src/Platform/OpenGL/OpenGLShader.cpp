#include "cbcpch.h"
#include "OpenGLShader.h"
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace cbc
{
	OpenGLShader::OpenGLShader(std::initializer_list<ShaderFile> shaders)
	{
		rendererID = glCreateProgram();
		RendererID* shaderIDs = new RendererID[shaders.size()];
		for (size_t i = 0; i < shaders.size(); i++)
		{
			shaderIDs[i] = Compile(*(shaders.begin() + i));
			glAttachShader(rendererID, shaderIDs[i]);
		}
	
		bool success = LinkAndValidate();
	
		// Do this regardles of success
		for (size_t i = 0; i < shaders.size(); i++)
		{
			glDetachShader(rendererID, shaderIDs[i]);
			glDeleteShader(shaderIDs[i]);
		}

		delete[] shaderIDs;

		if (!success)
		{
			CBC_CORE_WARN("Failed to create shader.");
			glDeleteProgram(rendererID);
			rendererID = 0;
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(rendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(rendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	bool OpenGLShader::LinkAndValidate()
	{
		// Link
		glLinkProgram(rendererID);
		int link;
		glGetProgramiv(rendererID, GL_LINK_STATUS, &link);
		if (link == GL_FALSE)
		{
			int length;
			glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetProgramInfoLog(rendererID, length, &length, message);
			CBC_CORE_WARN("Failed to link program: {0}", message);
			delete[] message;
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
			char* message = new char[length];
			glGetProgramInfoLog(rendererID, length, &length, message);
			CBC_CORE_WARN("Failed to validate program: {0}", message);
			delete[] message;
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
		}

		CBC_ASSERT(false, "Unknown Shader Type!");
		return 0;
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
		}

		CBC_ASSERT(false, "Unknown Shader Type!");
		return "unknown";
	}

	unsigned int OpenGLShader::Compile(const ShaderFile& shader)
	{
		RendererID id = glCreateShader(GetGLType(shader.type));
		const char* source = shader.source.c_str();
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);

		int compile;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compile);
		if (compile == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = new char[length];
			glGetShaderInfoLog(id, length, &length, message);
			CBC_CORE_WARN("Failed to compile {0} shader: {1}", GetName(shader.type), message);
			delete[] message;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		auto it = uniformLocations.find(name);
		if (it != uniformLocations.end())
			return it->second;

		int location = glGetUniformLocation(rendererID, name.c_str());

#if CBC_ENABLE_LOGGING // remove the if (location == -1) when logging is disabled
		if (location == -1)
			CBC_CORE_WARN("Unused shader uniform: {0}", name);
#endif
		uniformLocations[name] = location;
		return location;
	}

	void OpenGLShader::SetUniform(const std::string& name, float value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1f(location, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::vec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::vec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::vec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, int value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1i(location, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::ivec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::ivec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::ivec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, unsigned int value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1ui(location, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::uvec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2uiv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::uvec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3uiv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::uvec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4uiv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, bool value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1i(location, (int)value);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::bvec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2i(location, (int)value.x, (int)value.y);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::bvec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3i(location, (int)value.x, (int)value.y, (int)value.z);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::bvec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4i(location, (int)value.x, (int)value.y, (int)value.z, (int)value.w);
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat2x3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat2x4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat3x2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat3x4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4x2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4x3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetUniforms(const std::string& name, const int* values, int count)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetUniforms(const std::string& name, const unsigned int* values, int count)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1uiv(location, count, values);
	}
}
