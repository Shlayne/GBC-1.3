#include "Renderer.h"
#include <gl/glew.h>
#include "Basic/BasicRenderer.h"

void Renderer::Init()
{
	BasicRenderer::Init();
}

void Renderer::Shutdown()
{
	BasicRenderer::Shutdown();
}

int Renderer::GetMaxTextureSlots()
{
	int maxTextureSlots = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
	return maxTextureSlots;
}

int Renderer::GetMaxTextureSize()
{
	int maxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	return maxTextureSize;
}

int Renderer::GetMaxFramebufferSize()
{
	int maxFramebufferWidth = 0;
	int maxFramebufferHeight = 0;
	glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &maxFramebufferWidth);
	glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &maxFramebufferHeight);
	return maxFramebufferWidth * maxFramebufferHeight;
}

int Renderer::GetMaxFramebufferColorAttachments()
{
	int maxFramebufferColorAttachments = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxFramebufferColorAttachments);
	return maxFramebufferColorAttachments;
}

void Renderer::OnViewportResized(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::SetClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
	// TODO: abstract this
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EnableDepthTest(bool enable)
{
	if (enable) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}

void Renderer::EnableDepthMask(bool enable)
{
	if (enable) glDepthMask(GL_TRUE);
	else glDepthMask(GL_FALSE);
}

void Renderer::EnableCullFace(bool enable)
{
	if (enable) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void Renderer::EnableBlending(bool enable)
{
	if (enable)
	{
		glEnable(GL_BLEND);
		// TODO: this shouldn't go here
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);
}

void Renderer::DrawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, unsigned int maxIndexCount)
{
	vertexArray.Bind();
	indexBuffer.Bind();
	int indexCount = maxIndexCount != 0 ? maxIndexCount : indexBuffer.GetCount();

	// TODO: if unsigned shorts or chars are supported, that should be stored in IndexBuffer
	// TODO: support other primitive types, ex: GL_LINES_ADJACENCY
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}
