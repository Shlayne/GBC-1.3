#pragma once

#include <glm/glm.hpp>
#include "GBC/Rendering/VertexArray.h"

namespace gbc
{
	enum class RendererPrimitive
	{
		// OpenGL, Vulkan, Direct3D, and Metal support these
		Points,
		Lines,
		LineStrip,
		Triangles,
		TriangleStrip,

		// OpenGL, Vulkan, and Direct3D support these
		LinesAdjacency,
		LineStripAdjacency,
		TrianglesAdjacency,
		TriangleStripAdjacency,

		// OpenGL and Vulkan support these
		LineLoop,
		TriangleFan,
	};

	class RendererAPI
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		virtual void EnableBlending() = 0;
		virtual void DisableBlending() = 0;
		virtual void EnableCullFace() = 0;
		virtual void DisableCullFace() = 0;

		virtual void SetViewport(int x, int y, int width, int height) = 0;

		virtual void SetClearColor(float red, float green, float blue, float alpha) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, uint32_t offset, uint32_t count, RendererPrimitive primitive) = 0;

		// Renderer capabilities
		virtual int GetMaxTextureSlots() = 0;
		virtual int GetMaxTextureSize() = 0;
		virtual int GetMaxFramebufferWidth() = 0;
		virtual int GetMaxFramebufferHeight() = 0;
		virtual int GetMaxFramebufferColorAttachments() = 0;

		enum class API { Headless, OpenGL, Vulkan, Direct3D, Metal };
		static API GetAPI() { return api; }
	private:
		static API api;

		static Scope<RendererAPI> CreateScope();
		friend class Renderer;
	};
}
