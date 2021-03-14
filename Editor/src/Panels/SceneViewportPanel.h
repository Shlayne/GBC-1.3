#pragma once

#include "Panel.h"
#include "GBC/Rendering/Framebuffer.h"
#include "glm/glm.hpp"

namespace gbc
{
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(const std::string& name, const Ref<Framebuffer>& framebuffer);

		virtual void OnImGuiRender() override;

		bool HasViewportSizeChanged() const { return viewportSizeChanged; }
		const glm::vec2& GetViewportSize() const { return viewportSize; }
	private:
		Ref<Framebuffer> framebuffer;
		glm::vec2 viewportSize{0.0f};
		bool viewportSizeChanged = false;
	};
}
