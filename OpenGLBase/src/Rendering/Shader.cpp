#include "Shader.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace cbc
{
	Ref<Shader> Shader::CreateRef(std::initializer_list<ShaderFile> shaders)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateRef<OpenGLShader>(shaders);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(std::initializer_list<ShaderFile> shaders)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:   return nullptr;
			case RendererAPI::API::OpenGL: return cbc::CreateScope<OpenGLShader>(shaders);
		}

		CBC_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}
