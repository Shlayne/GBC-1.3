#pragma once

#include "GBC/Core/Keycodes.h"
#include "GBC/Core/Mousecodes.h"

namespace gbc
{
	class Input
	{
	public:
		static bool IsKeyPressed(Keycode keycode);
		static bool IsKeyReleased(Keycode keycode);

		static bool IsMouseButtonPressed(Mousecode button);
		static bool IsMouseButtonReleased(Mousecode button);

		static std::pair<float, float> GetMousePos();
		static float GetMousePosX();
		static float GetMousePosY();
	};
}
