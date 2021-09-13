#pragma once

#include <string>

namespace gbc
{
	struct TagComponent
	{
		TagComponent() noexcept = default;
		TagComponent(const std::string& tag);
		TagComponent(std::string&& tag) noexcept;
		TagComponent& operator=(const std::string& tag);
		TagComponent& operator=(std::string&& tag) noexcept;

		inline operator std::string&() noexcept { return tag; }
		inline operator const std::string&() const noexcept { return tag; }

		std::string tag;
	};
}
