#include "gbcpch.h"
#include "Renderer.h"
#include "Basic/BasicRenderer.h"

namespace gbc
{
	Scope<RendererAPI> Renderer::api = nullptr;

	void Renderer::Init()
	{
		GBC_PROFILE_FUNCTION();

		api = RendererAPI::CreateScope();
		api->Init();
		BasicRenderer::Init();
	}

	void Renderer::Shutdown()
	{
		GBC_PROFILE_FUNCTION();

		BasicRenderer::Shutdown();
		api->Shutdown();
	}

	void Renderer::EnableDepthTest()
	{ api->EnableDepthTest(); }

	void Renderer::DisableDepthTest()
	{ api->DisableDepthTest(); }

	void Renderer::EnableBlending()
	{ api->EnableBlending(); }

	void Renderer::DisableBlending()
	{ api->DisableBlending(); }

	void Renderer::EnableCullFace()
	{ api->EnableCullFace(); }

	void Renderer::DisableCullFace()
	{ api->DisableCullFace(); }

	void Renderer::SetViewport(const glm::ivec2& position, const glm::ivec2& size)
	{ api->SetViewport(position.x, position.y, size.x, size.y); }
	void Renderer::SetViewport(int x, int y, int width, int height)
	{ api->SetViewport(x, y, width, height); }

	void Renderer::Clear()
	{ api->Clear(); }

	void Renderer::SetClearColor(const glm::vec4& color)
	{ api->SetClearColor(color.r, color.g, color.b, color.a); }
	void Renderer::SetClearColor(float red, float green, float blue, float alpha)
	{ api->SetClearColor(red, green, blue, alpha); }

	void Renderer::DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, uint32_t offset, uint32_t count, RendererPrimitive primitive)
	{ api->DrawIndexed(vertexArray, indexBuffer, offset, count, primitive); }
}
