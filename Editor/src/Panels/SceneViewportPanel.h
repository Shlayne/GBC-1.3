#pragma once

#include "Panel.h"
#include <functional>
#include "glm/glm.hpp"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	using OpenSceneFunc = std::function<void(const std::string&)>;

	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(const std::string& name, Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, bool& canUseGizmos, EditorCamera& editorCamera, const OpenSceneFunc& openScene);

		virtual void OnImGuiRender() override;
	private:
		Ref<Framebuffer>& framebuffer;
		Ref<Scene>& context;
		Entity& selectedEntity;
		int& gizmoType;
		bool& canUseGizmos;
		EditorCamera& editorCamera;
		OpenSceneFunc openScene;
	};
}
