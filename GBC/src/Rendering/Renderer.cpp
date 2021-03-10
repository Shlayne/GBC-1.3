#include "gbcpch.h"
#include "Renderer.h"
#include "Rendering/Basic/BasicRenderer.h"

namespace gbc
{
	Scope<RendererAPI> Renderer::api = RendererAPI::CreateScope();

	void Renderer::Init()
	{
		BasicRenderer::Init();
		api->Init();
	}

	void Renderer::Shutdown()
	{
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

	int Renderer::GetMaxTextureSlots()
	{ return api->GetMaxTextureSlots(); }

	int Renderer::GetMaxTextureSize()
	{ return api->GetMaxTextureSize(); }

	int Renderer::GetMaxFramebufferWidth()
	{ return api->GetMaxFramebufferWidth(); }

	int Renderer::GetMaxFramebufferHeight()
	{ return api->GetMaxFramebufferHeight(); }

	int Renderer::GetMaxFramebufferColorAttachments()
	{ return api->GetMaxFramebufferColorAttachments(); }

	void Renderer::SetViewport(int x, int y, int width, int height)
	{ api->SetViewport(x, y, width, height); }

	void Renderer::Clear()
	{ api->Clear(); }

	void Renderer::SetClearColor(const glm::vec4& color)
	{ api->SetClearColor(color); }

	void Renderer::DrawIndexed(const Ref<VertexArray>& vertexArray, const Ref<IndexBuffer>& indexBuffer, unsigned int count)
	{ api->DrawIndexed(vertexArray, indexBuffer, count); }
}
