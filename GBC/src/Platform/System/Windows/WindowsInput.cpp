#include "gbcpch.h"
#include "GBC/Core/Input.h"
#include "GBC/Core/Application.h"
#include <glfw/glfw3.h>
#include <array>

namespace gbc
{
	struct InputData
	{
		// Key
		std::array<bool, GLFW_KEY_LAST + 1> keys{};

		// Mouse
		std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseButtons{};

		// Joystick
		std::array<JoystickState, GLFW_JOYSTICK_LAST + 1> joysticks{};

		// Callback
		EventCallbackFunc eventCallback;
	};
	static InputData data;

	void Input::PreInit()
	{
		data.keys.fill(false);
		data.mouseButtons.fill(false);

		glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
	}

	void Input::Init()
	{
		glfwSetJoystickCallback([](int32_t jid, int32_t deviceEvent)
		{
			switch (deviceEvent)
			{
				case GLFW_CONNECTED: OnJoystickConnected(jid); break;
				case GLFW_DISCONNECTED: OnJoystickDisconnected(jid); break;
			}
		});

		// Now that the joystick callback is set, emit some fake connect events
		// for joysticks that were connected prior to running the application.
		for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
			if (glfwJoystickPresent(jid) == GLFW_TRUE)
				OnJoystickConnected(jid);
	}

	void Input::OnJoystickConnected(int32_t jid)
	{
		uint32_t buttonCount = 0;
		uint32_t axisCount = 0;
		uint32_t hatCount = 0;

		(void)glfwGetJoystickHats(jid, (int32_t*)&hatCount);

		if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
		{
			buttonCount = sizeof(GLFWgamepadstate::buttons) / sizeof(*GLFWgamepadstate::buttons);
			axisCount = sizeof(GLFWgamepadstate::axes) / sizeof(*GLFWgamepadstate::axes);
		}
		else
		{
			(void)glfwGetJoystickButtons(jid, (int32_t*)&buttonCount);
			(void)glfwGetJoystickAxes(jid, (int32_t*)&axisCount);
		}

		data.joysticks[jid].OnConnect(buttonCount, axisCount, hatCount);
		JoystickConnectEvent event(jid, true);
		data.eventCallback(event);
	}

	void Input::OnJoystickDisconnected(int32_t jid)
	{
		data.joysticks[jid].OnDisconnect();
		JoystickConnectEvent event(jid, false);
		data.eventCallback(event);
	}

	void Input::Update()
	{
		for (int32_t jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
		{
			JoystickState& joystick = data.joysticks[jid];

			if (joystick.IsConnected())
			{
				GLFWgamepadstate state;

				int32_t buttonCount;
				int32_t axisCount;
				int32_t hatCount;

				const unsigned char* buttons;
				const float* axes;
				const unsigned char* hats = glfwGetJoystickHats(jid, &hatCount);

				if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
				{
					glfwGetGamepadState(jid, &state);
					buttons = state.buttons;
					axes = state.axes;
					buttonCount = sizeof(GLFWgamepadstate::buttons) / sizeof(*GLFWgamepadstate::buttons);
					axisCount = sizeof(GLFWgamepadstate::axes) / sizeof(*GLFWgamepadstate::axes);
				}
				else
				{
					buttons = glfwGetJoystickButtons(jid, &buttonCount);
					axes = glfwGetJoystickAxes(jid, &axisCount);
				}

				for (int32_t i = 0; i < buttonCount; i++)
					joystick.SetButton(static_cast<JoystickButton>(i), buttons[i]);
				for (int32_t i = 0; i < axisCount; i++)
					joystick.SetAxis(static_cast<JoystickAxis>(i), axes[i]);
				for (int32_t i = 0; i < hatCount; i++)
					joystick.SetHat(static_cast<JoystickHat>(i), static_cast<JoystickHatState>(hats[i]));
			}
		}
	}
	
	void Input::SetEventCallback(const EventCallbackFunc& callback)
	{
		data.eventCallback = callback;
	}

	bool Input::OnKeyPressEvent(KeyPressEvent& event)
	{
		data.keys[static_cast<size_t>(event.GetKeycode())] = true;
		return false;
	}

	bool Input::OnKeyReleaseEvent(KeyReleaseEvent& event)
	{
		data.keys[static_cast<size_t>(event.GetKeycode())] = false;
		return false;
	}

	bool Input::OnMouseButtonPressEvent(MouseButtonPressEvent& event)
	{
		data.mouseButtons[static_cast<size_t>(event.GetButton())] = true;
		return false;
	}

	bool Input::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event)
	{
		data.mouseButtons[static_cast<size_t>(event.GetButton())] = false;
		return false;
	}

	// Key

	bool Input::IsKeyPressed(Keycode keycode)
	{
		GBC_CORE_ASSERT(keycode < Keycode::Count, "Keycode index out of bounds!");
		return data.keys[static_cast<size_t>(keycode)];
	}

	bool Input::IsKeyReleased(Keycode keycode)
	{
		GBC_CORE_ASSERT(keycode < Keycode::Count, "Keycode index out of bounds!");
		return !data.keys[static_cast<size_t>(keycode)];
	}

	// Mouse

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		GBC_CORE_ASSERT(button < MouseButton::Count, "MouseButton index out of bounds!");
		return data.mouseButtons[static_cast<size_t>(button)];
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		GBC_CORE_ASSERT(button < MouseButton::Count, "MouseButton index out of bounds!");
		return !data.mouseButtons[static_cast<size_t>(button)];
	}

	glm::vec2 Input::GetAbsoluteMousePosition()
	{
		GLFWwindow* window = glfwGetCurrentContext();
		int32_t windowX, windowY;
		double cursorX, cursorY;
		glfwGetWindowPos(window, &windowX, &windowY);
		glfwGetCursorPos(window, &cursorX, &cursorY);
		return { static_cast<float>(windowX + cursorX), static_cast<float>(windowY + cursorY) };
	}

	glm::vec2 Input::GetRelativeMousePosition(void* nativeWindow)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(nativeWindow);
		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		return { static_cast<float>(cursorX), static_cast<float>(cursorY) };
	}

	// Joystick

	const JoystickState& Input::GetJoystickState(Joystick joystick)
	{
		GBC_CORE_ASSERT(joystick < Joystick::Count, "Joystick index out of bounds!");
		return data.joysticks[static_cast<size_t>(joystick)];
	}

	static bool IsJoystickButtonInState(Joystick joystick, JoystickButton button, bool buttonPressed)
	{
		GBC_CORE_ASSERT(joystick < Joystick::Count, "Joystick index out of bounds!");
		int32_t jid = static_cast<int>(joystick);
		const JoystickState& joystickState = data.joysticks[jid];
		return joystickState.IsConnected() && joystickState.GetButton(button) == buttonPressed;
	}

	bool Input::IsJoystickButtonPressed(Joystick joystick, JoystickButton button)
	{ return IsJoystickButtonInState(joystick, button, true); }

	bool Input::IsJoystickButtonReleased(Joystick joystick, JoystickButton button)
	{ return IsJoystickButtonInState(joystick, button, false); }

	float Input::GetJoystickAxis(Joystick joystick, JoystickAxis axis)
	{
		GBC_CORE_ASSERT(joystick < Joystick::Count, "Joystick index out of bounds!");
		int32_t jid = static_cast<int>(joystick);
		const JoystickState& joystickState = data.joysticks[jid];
		return joystickState.IsConnected() ? joystickState.GetAxis(axis) : 0.0f;
	}

	JoystickHatState Input::GetJoystickHat(Joystick joystick, JoystickHat hat)
	{
		GBC_CORE_ASSERT(joystick < Joystick::Count, "Joystick index out of bounds!");
		int32_t jid = static_cast<int>(joystick);
		const JoystickState& joystickState = data.joysticks[jid];
		return joystickState.IsConnected() ? joystickState.GetHat(hat) : JoystickHatState::Centered;
	}
}
