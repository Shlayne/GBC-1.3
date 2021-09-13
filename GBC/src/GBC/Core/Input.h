#pragma once

#include <glm/glm.hpp>
#include "GBC/Core/Joysticks.h"
#include "GBC/Core/Keycodes.h"
#include "GBC/Core/MouseButtons.h"
#include "GBC/Core/Window.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Events/MouseEvents.h"

namespace gbc
{
	class Input
	{
	public:
		static void PreInit();
		static void Init();
		static void Update();

		// Key
		static bool IsKeyPressed(Keycode keycode);
		static bool IsKeyReleased(Keycode keycode);

		// Mouse
		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		// Gets the position of the mouse relative to the top left corner of the primary monitor.
		static glm::vec2 GetAbsoluteMousePosition();
		// Gets the position of the mouse relative to the window associated with nativeWindow.
		static glm::vec2 GetRelativeMousePosition(void* nativeWindow);

		// Joystick
		static const JoystickState& GetJoystickState(Joystick joystick);

		static bool IsJoystickButtonPressed(Joystick joystick, JoystickButton button);
		static bool IsJoystickButtonReleased(Joystick joystick, JoystickButton button);

		static float GetJoystickAxis(Joystick joystick, JoystickAxis axis);

		static JoystickHatState GetJoystickHat(Joystick joystick, JoystickHat hat);
	private:
		friend class Application;
		static bool OnKeyPressEvent(KeyPressEvent& event);
		static bool OnKeyReleaseEvent(KeyReleaseEvent& event);
		static bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		static bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);

		static void OnJoystickConnected(int jid);
		static void OnJoystickDisconnected(int jid);
	};
}
