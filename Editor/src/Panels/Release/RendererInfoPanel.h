#pragma once

#include "Panels/Panel.h"

namespace gbc
{
	class RendererInfoPanel : public Panel
	{
	public:
		RendererInfoPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	private:
		const char* vendor;
		const char* renderer;
		const char* version;
	};
}
