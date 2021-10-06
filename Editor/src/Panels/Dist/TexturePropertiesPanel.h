#pragma once

#include "Panels/Panel.h"

namespace gbc
{
	class TexturePropertiesPanel : public Panel
	{
	public:
		TexturePropertiesPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	};
}
