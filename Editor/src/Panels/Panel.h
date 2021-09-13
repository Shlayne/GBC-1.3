#pragma once

#include "GBC/Core/Core.h"
#if GBC_ENABLE_IMGUI
#include <string>
#include "glm/glm.hpp"

namespace gbc
{
	class Panel
	{
	public:
		Panel() = default;
		Panel(const std::string& name);
		virtual ~Panel() = default;

		virtual void OnImGuiRender() = 0;

		inline bool IsEnabled() const noexcept { return enabled; }
		inline void SetEnabled(bool enabled) noexcept { this->enabled = enabled; }
		inline void ToggleEnabled() noexcept { enabled = !enabled; }

		inline bool IsFocused() const noexcept { return focused; }
		inline bool IsHovered() const noexcept { return hovered; }

		inline bool HasSizeChanged() const noexcept { return sizeChanged && size.x > 0 && size.y > 0; }
		inline const glm::ivec2& GetSize() const noexcept { return size; }
		inline const glm::vec2& GetPosition() const noexcept { return position; }

		void Update();
	protected:
		std::string name;
		bool enabled = true;
		bool focused = false;
		bool hovered = false;
		bool sizeChanged = false;
		glm::ivec2 size{ 1 };
		glm::vec2 position{ 0.0f };
	};
}
#endif
