#pragma once

#include "GBC/Core/Core.h"
#if GBC_ENABLE_STATS
#include "Panels/Panel.h"

namespace gbc
{
	class StatisticsPanel : public Panel
	{
	public:
		StatisticsPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	};
}
#endif
