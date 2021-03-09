#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

Ref<Shader> Shader::Create(std::initializer_list<ShaderFile> shaders)
{
	return CreateRef<OpenGLShader>(shaders);
}
