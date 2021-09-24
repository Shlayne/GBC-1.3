#pragma once

#include "GBC/Core/UUID.h"

namespace gbc
{
	struct IDComponent
	{
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(UUID id) : id(id) {}

		UUID id;
	};
}
