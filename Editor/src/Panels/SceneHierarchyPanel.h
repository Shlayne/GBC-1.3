#pragma once

#include "Panel.h"
#include "GBC/Scene/Scene.h"
#include "GBC/Scene/Entity.h"
#include "GBC/Core/Core.h"

namespace gbc
{
	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(const std::string& name, const Ref<Scene>& context);
		
		void SetContext(const Ref<Scene>& context);

		virtual void OnImGuiRender() override;

		inline Entity& GetSelectedEntity() { return selectedEntity; }
	private:
		void DrawEntityNode(Entity entity);

		Ref<Scene> context;
		Entity selectedEntity;
	};
}
