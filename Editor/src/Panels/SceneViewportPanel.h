#pragma once

#include "Panel.h"
#include "GBC/Rendering/Framebuffer.h"
#include "glm/glm.hpp"
#include "GBC/Scene/Entity.h"
#include "GBC/Rendering/EditorCamera.h"

namespace gbc
{
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(const std::string& name, const Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, EditorCamera& editorCamera);

		virtual void OnImGuiRender() override;

		inline bool IsViewportFocused() const { return viewportFocused; }
		inline bool IsViewportHovered() const { return viewportHovered; }
		inline bool HasViewportSizeChanged() const { return viewportSizeChanged; }
		inline const glm::vec2& GetViewportSize() const { return viewportSize; }
	private:
		Ref<Framebuffer> framebuffer;
		glm::vec2 viewportSize{0.0f};
		bool viewportSizeChanged = false;
		bool viewportFocused = false;
		bool viewportHovered = false;

		Ref<Scene>& context;
		Entity& selectedEntity;
		int& gizmoType;
		EditorCamera& editorCamera;
	};
}
