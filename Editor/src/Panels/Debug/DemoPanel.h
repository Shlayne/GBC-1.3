#pragma once

#include "Panels/Panel.h"

namespace gbc
{
	class DemoPanel : public Panel
	{
	public:
		DemoPanel(const std::string& name);

		virtual void OnImGuiRender() override;
	};
}
