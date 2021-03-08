#include "Renderer.h"
#include "Basic/BasicRenderer.h"

#include <gl/glew.h>

void Renderer::init()
{
	BasicRenderer::init();
}

void Renderer::shutdown()
{
	BasicRenderer::shutdown();
}

int Renderer::getMaxTextureSlots()
{
	int maxTextureSlots = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
	return maxTextureSlots;
}

int Renderer::getMaxTextureSize()
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

void Renderer::onViewportResized(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Renderer::setClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
	// TODO: abstract this
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::enableDepthTest(bool enable)
{
	if (enable) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}

void Renderer::enableDepthMask(bool enable)
{
	if (enable) glDepthMask(GL_TRUE);
	else glDepthMask(GL_FALSE);
}

void Renderer::enableCullFace(bool enable)
{
	if (enable) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void Renderer::enableBlending(bool enable)
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

void Renderer::drawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, unsigned int maxIndexCount)
{
	vertexArray.bind();
	indexBuffer.bind();
	int indexCount = maxIndexCount != 0 ? maxIndexCount : indexBuffer.getCount();

	// TODO: if unsigned shorts or chars are supported, that should be stored in IndexBuffer
	// TODO: support other primitive types, ex: GL_LINES_ADJACENCY
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}
