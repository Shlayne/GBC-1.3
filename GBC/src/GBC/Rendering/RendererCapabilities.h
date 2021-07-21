#pragma once

namespace gbc
{
	class RendererCapabilities
	{
	public:
		static int GetMaxTextureSlots();
		static int GetMaxTextureSize();
		static int GetMaxFramebufferWidth();
		static int GetMaxFramebufferHeight();
		static int GetMaxFramebufferColorAttachments();
	};
}
