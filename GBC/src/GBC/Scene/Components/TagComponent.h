#pragma once

#include <string>

namespace gbc
{
	struct TagComponent
	{
		constexpr TagComponent() noexcept = default;
		constexpr TagComponent(const std::string& tag);
		constexpr TagComponent(std::string&& tag) noexcept;
		constexpr TagComponent& operator=(const std::string& tag);
		constexpr TagComponent& operator=(std::string&& tag) noexcept;

		std::string tag;
	};
}

#include "TagComponent.inl"
