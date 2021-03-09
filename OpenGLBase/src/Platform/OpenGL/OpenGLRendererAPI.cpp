#include "OpenGLRendererAPI.h"
#include <gl/glew.h>

namespace cbc
{
	void OpenGLRendererAPI::Init()
	{
		// TODO: this should be abstracted (shouldn't go here)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Shutdown()
	{

	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::EnableBlending()
	{
		glEnable(GL_BLEND);
	}

	void OpenGLRendererAPI::DisableBlending()
	{
		glDisable(GL_BLEND);
	}

	void OpenGLRendererAPI::EnableCullFace()
	{
		glEnable(GL_CULL_FACE);
	}

	void OpenGLRendererAPI::DisableCullFace()
	{
		glDisable(GL_CULL_FACE);
	}

	int OpenGLRendererAPI::GetMaxTextureSlots()
	{
		int maxTextureSlots = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
		return maxTextureSlots;
	}

	int OpenGLRendererAPI::GetMaxTextureSize()
	{
		int maxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		return maxTextureSize;
	}

	int OpenGLRendererAPI::GetMaxFramebufferWidth()
	{
		int maxFramebufferWidth = 0;
		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
		return maxFramebufferWidth;
	}

	int OpenGLRendererAPI::GetMaxFramebufferHeight()
	{
		int maxFramebufferHeight = 0;
		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
		return maxFramebufferHeight;
	}

	int OpenGLRendererAPI::GetMaxFramebufferColorAttachments()
	{
		int maxFramebufferColorAttachments = 0;
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxFramebufferColorAttachments);
		return maxFramebufferColorAttachments;
	}

	void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		// TODO: abstract this
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, unsigned int count)
	{
		vertexArray->Bind();
		indexBuffer->Bind();
		int indexCount = count != 0 ? count : indexBuffer->GetCount();

		// TODO: if unsigned shorts or unsigned chars are supported, that should be stored in IndexBuffer
		// TODO: support other primitive types, ex: GL_LINES_ADJACENCY
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}
