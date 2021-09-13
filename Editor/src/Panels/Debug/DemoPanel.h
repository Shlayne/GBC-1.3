#pragma once

#include "Panels/Panel.h"
#if GBC_CONFIG_DEBUG
namespace gbc
{
	class DemoPanel : public Panel
	{
	public:
		DemoPanel(const std::string& name);

		virtual void OnImGuiRender() override;
	};
}
#endif
