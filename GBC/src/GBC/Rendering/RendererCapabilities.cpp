#include "gbcpch.h"
#include "RendererCapabilities.h"
#include "Renderer.h"

namespace gbc
{
	int RendererCapabilities::GetMaxTextureSlots()
	{ return Renderer::api->GetMaxTextureSlots(); }

	int RendererCapabilities::GetMaxTextureSize()
	{ return Renderer::api->GetMaxTextureSize(); }

	int RendererCapabilities::GetMaxFramebufferWidth()
	{ return Renderer::api->GetMaxFramebufferWidth(); }

	int RendererCapabilities::GetMaxFramebufferHeight()
	{ return Renderer::api->GetMaxFramebufferHeight(); }

	int RendererCapabilities::GetMaxFramebufferColorAttachments()
	{ return Renderer::api->GetMaxFramebufferColorAttachments(); }
}
