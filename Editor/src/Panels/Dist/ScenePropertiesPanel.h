#pragma once

#include "Panels/Panel.h"

namespace gbc
{
	class ScenePropertiesPanel : public Panel
	{
	public:
		ScenePropertiesPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	};
}
