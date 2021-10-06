#pragma once

#include "GBC/Core/Core.h"
#if GBC_CONFIG_DEBUG
#include "Panels/Panel.h"

namespace gbc
{
	class DemoPanel : public Panel
	{
	public:
		DemoPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	};
}
#endif
