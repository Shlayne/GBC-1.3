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
		SceneViewportPanel(const std::string& name, bool& viewportSizeChanged, bool& viewportFocused, bool& viewportHovered, glm::ivec2& viewportSize, glm::vec2& viewportPos, glm::vec2& absoluteMousePos, Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, EditorCamera& editorCamera);

		virtual void OnImGuiRender() override;
	private:
		bool& viewportSizeChanged;
		bool& viewportFocused;
		bool& viewportHovered;
		glm::ivec2& viewportSize;
		glm::vec2& viewportPos;
		glm::vec2& absoluteMousePos;
		Ref<Framebuffer>& framebuffer;
		Ref<Scene>& context;
		Entity& selectedEntity;
		int& gizmoType;
		EditorCamera& editorCamera;
	};
}
