#pragma once

#include "Panels/Panel.h"
#include "GBC/Core/Core.h"
#include "GBC/Scene/Entity.h"
#include "GBC/Scene/Scene.h"

namespace gbc
{
	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(const std::string& name, Ref<Scene>& context, Entity& selectedEntity);

		virtual void OnImGuiRender() override;
	private:
		void DrawEntityNode(Entity entity);

		Ref<Scene>& context;
		Entity& selectedEntity;
	};
}
