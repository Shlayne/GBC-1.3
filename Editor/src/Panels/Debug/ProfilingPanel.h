#pragma once

#include "GBC/Core/Core.h"
#if GBC_ENABLE_PROFILE_RUNTIME
#include "Panels/Panel.h"

namespace gbc
{
	class ProfilingPanel : public Panel
	{
	public:
		ProfilingPanel(const std::string& name);

		virtual void OnImGuiRender() override;
	private:
		bool profiling = false;
		uint32_t profileCount = 0;
	};
}
#endif
