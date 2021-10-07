#pragma once

#include "GBC/Events/Event.h"
#include <glm/glm.hpp>
#include <string>

namespace gbc
{
	class EditorLayer;

	class Panel
	{
	public:
		constexpr Panel() noexcept = default;
		Panel(const std::string& name, EditorLayer* editorLayer);
		virtual ~Panel() = default;

		virtual void OnImGuiRender() {}

		constexpr const std::string& GetName() const noexcept { return name; }

		constexpr bool IsEnabled() const noexcept { return enabled; }
		constexpr void SetEnabled(bool enabled) noexcept { this->enabled = enabled; }
		constexpr void ToggleEnabled() noexcept { enabled = !enabled; }

		constexpr bool IsFocused() const noexcept { return focused; }
		constexpr bool IsHovered() const noexcept { return hovered; }

		constexpr bool HasSizeChanged() const noexcept { return sizeChanged && size.x > 0 && size.y > 0; }
		constexpr const glm::ivec2& GetSize() const noexcept { return size; }
		constexpr const glm::ivec2& GetPosition() const noexcept { return position; }
	protected:
		void Update();

		std::string name;
		EditorLayer* editorLayer = nullptr;

		bool enabled = true;
		bool focused = false;
		bool hovered = false;
		bool sizeChanged = false;
		glm::ivec2 size{ 1 };
		glm::ivec2 position{ 0 };
	};
}
