#pragma once

#include "Panels/Panel.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	class SceneHierarchyPanel : public Panel
	{
	public:
		SceneHierarchyPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	private:
		void DrawEntityNode(Entity entity);
		void MakeCurrentItemDragDropSource(Entity source);
		void MakeCurrentItemDragDropTarget(Entity target);

		Entity entityToDelete;
	};
}
