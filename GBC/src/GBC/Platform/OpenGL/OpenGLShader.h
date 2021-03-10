#pragma once

#include "GBC/Rendering/Shader.h"

namespace gbc
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::initializer_list<ShaderFile> shaders);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform (const std::string& name, float              value) override;
		virtual void SetUniform (const std::string& name, const glm::vec2&   value) override;
		virtual void SetUniform (const std::string& name, const glm::vec3&   value) override;
		virtual void SetUniform (const std::string& name, const glm::vec4&   value) override;
		virtual void SetUniform (const std::string& name, int                value) override;
		virtual void SetUniform (const std::string& name, const glm::ivec2&  value) override;
		virtual void SetUniform (const std::string& name, const glm::ivec3&  value) override;
		virtual void SetUniform (const std::string& name, const glm::ivec4&  value) override;
		virtual void SetUniform (const std::string& name, unsigned int       value) override;
		virtual void SetUniform (const std::string& name, const glm::uvec2&  value) override;
		virtual void SetUniform (const std::string& name, const glm::uvec3&  value) override;
		virtual void SetUniform (const std::string& name, const glm::uvec4&  value) override;
		virtual void SetUniform (const std::string& name, bool               value) override;
		virtual void SetUniform (const std::string& name, const glm::bvec2&  value) override;
		virtual void SetUniform (const std::string& name, const glm::bvec3&  value) override;
		virtual void SetUniform (const std::string& name, const glm::bvec4&  value) override;
		virtual void SetUniform (const std::string& name, const glm::mat2&   value) override;
		virtual void SetUniform (const std::string& name, const glm::mat2x3& value) override;
		virtual void SetUniform (const std::string& name, const glm::mat2x4& value) override;
		virtual void SetUniform (const std::string& name, const glm::mat3x2& value) override;
		virtual void SetUniform (const std::string& name, const glm::mat3&   value) override;
		virtual void SetUniform (const std::string& name, const glm::mat3x4& value) override;
		virtual void SetUniform (const std::string& name, const glm::mat4x2& value) override;
		virtual void SetUniform (const std::string& name, const glm::mat4x3& value) override;
		virtual void SetUniform (const std::string& name, const glm::mat4&   value) override;
		virtual void SetUniforms(const std::string& name, const int*          values, int count) override;
		virtual void SetUniforms(const std::string& name, const unsigned int* values, int count) override;
	private:
		bool LinkAndValidate();
		RendererID Compile(const ShaderFile& shader);
		int GetUniformLocation(const std::string& name) const;

		RendererID rendererID = 0;
		mutable std::unordered_map<std::string, int> uniformLocations;
	};
}
