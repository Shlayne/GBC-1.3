#pragma once

#include <string>
#include "GBC/Core/Core.h"

#if GBC_ENABLE_IMGUI
namespace gbc
{
	class Panel
	{
	public:
		Panel() = default;
		Panel(const std::string& name);
		virtual ~Panel() = default;

		virtual void OnImGuiRender() = 0;

		inline bool IsEnabled() const { return enabled; }
		inline void SetEnabled(bool enabled) { this->enabled = enabled; }
		inline void ToggleEnabled() { enabled = !enabled; }

		inline bool IsFocused() const { return focused; }
		inline bool IsHovered() const { return hovered; }
	protected:
		std::string name;
		bool enabled = true;
		bool focused = false;
		bool hovered = false;
	};
}
#endif
