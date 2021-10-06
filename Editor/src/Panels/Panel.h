#pragma once

#include <glm/glm.hpp>
#include <string>

namespace gbc
{
	class EditorLayer;

	class Panel
	{
	public:
		Panel() = default;
		Panel(const std::string& name, EditorLayer* editorLayer);
		virtual ~Panel() = default;

		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const noexcept { return name; }

		inline bool IsEnabled() const noexcept { return enabled; }
		inline void SetEnabled(bool enabled) noexcept { this->enabled = enabled; }
		inline void ToggleEnabled() noexcept { enabled = !enabled; }

		inline bool IsFocused() const noexcept { return focused; }
		inline bool IsHovered() const noexcept { return hovered; }

		inline bool HasSizeChanged() const noexcept { return sizeChanged && size.x > 0 && size.y > 0; }
		inline const glm::ivec2& GetSize() const noexcept { return size; }
		inline const glm::ivec2& GetPosition() const noexcept { return position; }
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
