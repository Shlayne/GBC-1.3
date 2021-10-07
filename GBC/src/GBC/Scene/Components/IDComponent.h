#pragma once

#include "GBC/Core/UUID.h"

namespace gbc
{
	struct IDComponent
	{
		IDComponent() noexcept = default;
		constexpr IDComponent(const IDComponent&) noexcept = default;
		constexpr IDComponent(UUID id) noexcept : id(id) {}

		UUID id;
	};
}
