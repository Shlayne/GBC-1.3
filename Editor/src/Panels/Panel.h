#pragma once

#include <string>
#include "GBC/Core/Core.h"

#if GBC_ENABLE_IMGUI
namespace gbc
{
	class Panel
	{
	public:
		Panel(const std::string& name);
		virtual ~Panel() = default;

		virtual void OnImGuiRender() = 0;

		inline bool IsEnabled() const { return enabled; }
		inline void SetEnabled(bool enabled) { this->enabled = enabled; }
		inline void ToggleEnabled() { enabled = !enabled; }
	protected:
		std::string name;
		bool enabled = true;
	};
}
#endif
