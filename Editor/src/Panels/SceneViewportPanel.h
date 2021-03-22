#pragma once

#include "Panel.h"
#include "glm/glm.hpp"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(const std::string& name, bool& viewportSizeChanged, glm::ivec2& viewportSize, glm::vec2& viewportPos, glm::vec2& absoluteMousePos, Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, bool& canUseGizmos, EditorCamera& editorCamera);

		virtual void OnImGuiRender() override;
	private:
		bool& viewportSizeChanged;
		glm::ivec2& viewportSize;
		glm::vec2& viewportPos;
		glm::vec2& absoluteMousePos;
		Ref<Framebuffer>& framebuffer;
		Ref<Scene>& context;
		Entity& selectedEntity;
		int& gizmoType;
		bool& canUseGizmos;
		EditorCamera& editorCamera;
	};
}
