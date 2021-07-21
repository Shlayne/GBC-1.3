#include "gbcpch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "GBC/IO/FileIO.h"

namespace gbc
{
	static ShaderType GetType(const std::string& type)
	{
		
		if (type == "vertex")                 return ShaderType::Vertex;
		if (type == "tessolation control")    return ShaderType::TessolationControl;
		if (type == "tessolation evaluation") return ShaderType::TessolationEvaluation;
		if (type == "geometry")               return ShaderType::Geometry;
		if (type == "fragment")               return ShaderType::Fragment;
		if (type == "compute")                return ShaderType::Compute;

		GBC_CORE_ASSERT(false, "Unknown Shader Type!");
		return ShaderType::None;
	}

	static GLenum GetOpenGLType(ShaderType type)
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

		GBC_CORE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		CreateProgram(ParseFile(filepath));
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

	std::vector<ShaderFile> OpenGLShader::ParseFile(const std::string& filepath)
	{
		std::vector<ShaderFile> shaders;

		std::optional<std::string> file = FileIO::ReadFile(filepath);
		GBC_CORE_ASSERT(file, "Could not read shader file!");

		static constexpr const char* typeToken = "#type";
		static const size_t typeTokenLength = strlen(typeToken);
		size_t position = file->find(typeToken, 0);

		while (position != std::string::npos)
		{
			size_t lineEnd = file->find_first_of("\r\n", position);
			GBC_CORE_ASSERT(lineEnd != std::string::npos, "Cannot have #type at the end of the file!");

			size_t typeStart = position + typeTokenLength + 1;
			std::string typeString = file->substr(typeStart, lineEnd - typeStart);
			ShaderType type = GetType(typeString);

			size_t nestLineStart = file->find_first_not_of("\r\n", lineEnd);
			GBC_CORE_ASSERT(nestLineStart != std::string::npos, "Must include code for a shader!");
			position = file->find(typeToken, nestLineStart);
			shaders.push_back({type, position == std::string::npos ? file->substr(nestLineStart) : file->substr(nestLineStart, position - nestLineStart)});
		}

		return shaders;
	}

	void OpenGLShader::CreateProgram(const std::vector<ShaderFile>& shaders)
	{
		rendererID = glCreateProgram();
		RendererID* shaderIDs = new RendererID[shaders.size()];
		for (size_t i = 0; i < shaders.size(); i++)
		{
			shaderIDs[i] = CompileShader(shaders[i]);
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
			GBC_CORE_WARN("Failed to create shader.");
			glDeleteProgram(rendererID);
			rendererID = 0;
		}
	}

	RendererID OpenGLShader::CompileShader(const ShaderFile& shader)
	{
		RendererID id = glCreateShader(GetOpenGLType(shader.type));
		const GLchar* source = static_cast<const GLchar*>(shader.source.c_str());
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);

		GLint compile;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compile);
		if (compile == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			GLchar* message = new GLchar[length];
			glGetShaderInfoLog(id, length, &length, message);
			GBC_CORE_WARN("{0}", static_cast<char*>(message));
			delete[] message;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	bool OpenGLShader::LinkAndValidate()
	{
		// Link
		glLinkProgram(rendererID);
		GLint link;
		glGetProgramiv(rendererID, GL_LINK_STATUS, &link);
		if (link == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &length);
			GLchar* message = new GLchar[length];
			glGetProgramInfoLog(rendererID, length, &length, message);
			GBC_CORE_WARN("{0}", static_cast<char*>(message));
			delete[] message;
			return false;
		}

		// Validate
		glValidateProgram(rendererID);
		GLint validate;
		glGetProgramiv(rendererID, GL_VALIDATE_STATUS, &validate);
		if (validate == GL_FALSE)
		{
			GLint length;
			glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &length);
			GLchar* message = new GLchar[length];
			glGetProgramInfoLog(rendererID, length, &length, message);
			GBC_CORE_WARN("{0}", static_cast<char*>(message));
			delete[] message;
			return false;
		}

		return true;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		auto it = uniformLocations.find(name);
		if (it != uniformLocations.end())
			return it->second;

		int location = glGetUniformLocation(rendererID, name.c_str());
#if GBC_ENABLE_LOGGING // to remove warning of empty if statement in release and dist
		if (location == -1)
			GBC_CORE_WARN("Unused shader uniform: {0}", name);
#endif
		uniformLocations[name] = location;
		return location;
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1i(location, value);
	}

	void OpenGLShader::SetInt2(const std::string& name, const glm::ivec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetInt3(const std::string& name, const glm::ivec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetInt4(const std::string& name, const glm::ivec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4iv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUInt(const std::string& name, uint32_t value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1ui(location, value);
	}

	void OpenGLShader::SetUInt2(const std::string& name, const glm::uvec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2uiv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUInt3(const std::string& name, const glm::uvec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3uiv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetUInt4(const std::string& name, const glm::uvec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4uiv(location, 1, glm::value_ptr(value));
	}

	void OpenGLShader::SetBool(const std::string& name, bool value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1i(location, (int)value);
	}

	void OpenGLShader::SetBool2(const std::string& name, const glm::bvec2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform2i(location, (int)value.x, (int)value.y);
	}

	void OpenGLShader::SetBool3(const std::string& name, const glm::bvec3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform3i(location, (int)value.x, (int)value.y, (int)value.z);
	}

	void OpenGLShader::SetBool4(const std::string& name, const glm::bvec4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform4i(location, (int)value.x, (int)value.y, (int)value.z, (int)value.w);
	}

	void OpenGLShader::SetMat2(const std::string& name, const glm::mat2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat2x3(const std::string& name, const glm::mat2x3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat2x4(const std::string& name, const glm::mat2x4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat3x2(const std::string& name, const glm::mat3x2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat3x4(const std::string& name, const glm::mat3x4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4x2(const std::string& name, const glm::mat4x2& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4x3(const std::string& name, const glm::mat4x3& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetInts(const std::string& name, const int* values, int count)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetUInts(const std::string& name, const uint32_t* values, int count)
	{
		int location = GetUniformLocation(name);
		if (location != -1) glUniform1uiv(location, count, values);
	}
}
