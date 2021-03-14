#pragma once

#include "GBC/Core/Core.h"
#if GBC_ENABLE_STATS
#include "Panel.h"
#include "GBC/Rendering/Basic/BasicRenderer.h"

namespace gbc
{
	class StatisticsPanel : public Panel
	{
	public:
		StatisticsPanel(const std::string& name, const BasicRenderer::Statistics& statistics);

		virtual void OnImGuiRender() override;
	private:
		const BasicRenderer::Statistics& statistics;
	};
}
#endif
