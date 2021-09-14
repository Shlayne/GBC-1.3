#pragma once

#include "Panels/Panel.h"
#include "GBC/Scene/Entity.h"

namespace gbc
{
	class ScenePropertiesPanel : public Panel
	{
	public:
		ScenePropertiesPanel(const std::string& name, Entity& selectedEntity);

		virtual void OnImGuiRender() override;
	private:
		Entity& selectedEntity;
	};
}
