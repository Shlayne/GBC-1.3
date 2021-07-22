#include "gbcpch.h"
#include "OpenGLRendererAPI.h"
#include "glad/glad.h"

namespace gbc
{
	static GLenum GetOpenGLPrimitive(RendererPrimitive primitive)
	{
		switch (primitive)
		{
			case RendererPrimitive::Points:					return GL_POINTS;
			case RendererPrimitive::Lines:					return GL_LINES;
			case RendererPrimitive::LineStrip:				return GL_LINE_STRIP;
			case RendererPrimitive::Triangles:				return GL_TRIANGLES;
			case RendererPrimitive::TriangleStrip:			return GL_TRIANGLE_STRIP;
			case RendererPrimitive::LinesAdjacency:			return GL_LINES_ADJACENCY;
			case RendererPrimitive::LineStripAdjacency:		return GL_LINE_STRIP_ADJACENCY;
			case RendererPrimitive::TrianglesAdjacency:		return GL_TRIANGLES_ADJACENCY;
			case RendererPrimitive::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
			case RendererPrimitive::LineLoop:				return GL_LINE_LOOP;
			case RendererPrimitive::TriangleFan:			return GL_TRIANGLE_FAN;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderer Primitive!");
		return 0;
	}

	static GLenum GetOpenGLIndexType(IndexBufferElementType indexType)
	{
		switch (indexType)
		{
			case IndexBufferElementType::UInt32: return GL_UNSIGNED_INT;
			case IndexBufferElementType::UInt16: return GL_UNSIGNED_SHORT;
			case IndexBufferElementType::UInt8:  return GL_UNSIGNED_BYTE;
		}

		GBC_CORE_ASSERT(false, "Unknown Buffer Index Type!");
		return 0;
	}

	void OpenGLRendererAPI::Init()
	{
		clearBits |= GL_COLOR_BUFFER_BIT;

		// TODO: this should be abstracted (shouldn't go here)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::Shutdown()
	{

	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// TODO: this should include GL_STENCIL_BUFFER_BIT if using the stencil buffer
		clearBits |= GL_DEPTH_BUFFER_BIT;
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS);
		// TODO: this should include GL_STENCIL_BUFFER_BIT if using the stencil buffer
		clearBits &= ~GL_DEPTH_BUFFER_BIT;
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

	void OpenGLRendererAPI::SetClearColor(float red, float green, float blue, float alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void OpenGLRendererAPI::Clear()
	{
		// TODO: abstract this
		glClear(clearBits);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, uint32_t offset, uint32_t count, RendererPrimitive primitive)
	{
		vertexArray->Bind();
		indexBuffer->Bind();
		int indexCount = count != 0 ? count : indexBuffer->GetCount();
		glDrawElements(GetOpenGLPrimitive(primitive), indexCount, GetOpenGLIndexType(indexBuffer->GetType()), (const void*)(offset * static_cast<GLsizeiptr>(indexBuffer->GetType())));
	}

	// Renderer capabilities

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
