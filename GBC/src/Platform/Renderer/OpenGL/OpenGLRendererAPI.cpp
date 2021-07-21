#include "gbcpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace gbc
{
	static GLenum GetOpenGLPrimitive(RendererPrimitive primitive)
	{
		switch (primitive)
		{
			case RendererPrimitive::Triangles: return GL_TRIANGLES;
			case RendererPrimitive::Lines:     return GL_LINES;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer Primitive!");
		return 0;
	}

	static GLenum GetOpenGLIndexType(BufferIndexType indexType)
	{
		switch (indexType)
		{
			case BufferIndexType::UInt32: return GL_UNSIGNED_INT;
			case BufferIndexType::UInt16: return GL_UNSIGNED_SHORT;
			case BufferIndexType::UInt8:  return GL_UNSIGNED_BYTE;
		}

		GBC_CORE_ASSERT(false, "Unknown Buffer Index Type!");
		return 0;
	}

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
		glDepthFunc(GL_ALWAYS);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
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

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, uint32_t offset, uint32_t count, RendererPrimitive primitive)
	{
		vertexArray->Bind();
		indexBuffer->Bind();
		int indexCount = count != 0 ? count : indexBuffer->GetCount();
		glDrawElements(GetOpenGLPrimitive(primitive), indexCount, GetOpenGLIndexType(indexBuffer->GetType()), (const void*)(offset * static_cast<GLsizeiptr>(indexBuffer->GetType())));
	}

	// Render capabilities

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
}
