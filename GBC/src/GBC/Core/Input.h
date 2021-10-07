#pragma once

#include "GBC/Core/Joysticks.h"
#include "GBC/Core/Keycodes.h"
#include "GBC/Core/MouseButtons.h"
#include "GBC/Core/Window.h"
#include "GBC/Events/KeyEvents.h"
#include "GBC/Events/MouseEvents.h"
#include <glm/glm.hpp>

namespace gbc
{
	class Input
	{
	public:
		// Returns true if the key is currently pressed, false otherwise
		static bool IsKeyPressed(Keycode keycode);
		// Returns true if the key is currently released, false otherwise
		static bool IsKeyReleased(Keycode keycode);

		// Returns true if the mouse button is currently pressed, false otherwise
		static bool IsMouseButtonPressed(MouseButton button);
		// Returns true if the mouse button is currently released, false otherwise
		static bool IsMouseButtonReleased(MouseButton button);

		// Gets the position of the mouse relative to the top left corner of the primary monitor.
		static glm::vec2 GetAbsoluteMousePosition();
		// Gets the position of the mouse relative to the top left of nativeWindow's content area.
		static glm::vec2 GetRelativeMousePosition(void* nativeWindow);

		// Returns the current state of a joystick
		static const JoystickState& GetJoystickState(Joystick joystick);
		// Returns true if the joystick button is currently pressed, false otherwise
		static bool IsJoystickButtonPressed(Joystick joystick, JoystickButton button);
		// Returns true if the joystick button is currently released, false otherwise
		static bool IsJoystickButtonReleased(Joystick joystick, JoystickButton button);
		// Returns the current value of a joystick axis
		static float GetJoystickAxis(Joystick joystick, JoystickAxis axis);
		// Returns the current value of a joystick hat
		static JoystickHatState GetJoystickHat(Joystick joystick, JoystickHat hat);
	private:
		friend class Application;

		static void PreInit();
		static void Init();
		static void Update();

		static void SetEventCallback(const EventCallbackFunc& callback);
		static bool OnKeyPressEvent(KeyPressEvent& event);
		static bool OnKeyReleaseEvent(KeyReleaseEvent& event);
		static bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		static bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);

		static void OnJoystickConnected(Joystick joystick);
		static void OnJoystickDisconnected(Joystick joystick);
	};
}
