#include "gbcpch.h"
#include "Shader.h"
#include "RendererAPI.h"
#include "GBC/Platform/OpenGL/OpenGLShader.h"

namespace gbc
{
	Ref<Shader> Shader::CreateRef(std::initializer_list<ShaderFile> shaders)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateRef<OpenGLShader>(shaders);
		}

		GBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(std::initializer_list<ShaderFile> shaders)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return gbc::CreateScope<OpenGLShader>(shaders);
		}

		GBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
