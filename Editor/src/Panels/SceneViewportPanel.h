#pragma once

#include "Panels/Panel.h"
#include <glm/glm.hpp>
#include <functional>
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	using OpenSceneFunc = std::function<void(const std::filesystem::path&)>;

	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(const std::string& name, Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, bool& canUseGizmos, bool& canRenderGizmos, EditorCamera& editorCamera, const OpenSceneFunc& openScene);

		virtual void OnImGuiRender() override;
	private:
		Ref<Framebuffer>& framebuffer;
		Ref<Scene>& context;
		Entity& selectedEntity;
		int& gizmoType;
		bool& canUseGizmos;
		bool& canRenderGizmos;
		EditorCamera& editorCamera;
		OpenSceneFunc openScene;
	};
}
