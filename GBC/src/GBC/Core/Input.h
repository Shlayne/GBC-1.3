#pragma once

#include "GBC/Core/Keycodes.h"
#include "GBC/Core/Mousecodes.h"
#include "glm/glm.hpp"

namespace gbc
{
	class Input
	{
	public:
		static bool IsKeyPressed(Keycode keycode);
		static bool IsKeyReleased(Keycode keycode);

		static bool IsMouseButtonPressed(Mousecode button);
		static bool IsMouseButtonReleased(Mousecode button);

		static glm::vec2 GetMousePos();
		static float GetMousePosX();
		static float GetMousePosY();
	};
}
