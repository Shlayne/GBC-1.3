#include "gbcpch.h"
#include "GBC/Core/Input.h"
#include "glfw/glfw3.h"
#include <array>
#include "GBC/Core/Application.h"

namespace gbc
{
	struct InputData
	{
		// Key
		std::array<bool, GLFW_KEY_LAST + 1> keys;

		// Mouse
		std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> mouseButtons;

		// Joystick
		std::array<JoystickState, GLFW_JOYSTICK_LAST + 1> joysticks;

		// Misc.
		GLFWwindow* lastContext = nullptr;
	};
	static InputData data;

	void Input::PreInit()
	{
		data.keys.fill(0);
		data.mouseButtons.fill(false);

		glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_FALSE);
	}

	void Input::Init()
	{
		glfwSetJoystickCallback([](int jid, int deviceEvent)
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

	void Input::OnJoystickConnected(int jid)
	{
		uint32_t buttonCount = 0;
		uint32_t axisCount = 0;
		uint32_t hatCount = 0;

		(void)glfwGetJoystickHats(jid, (int*)&hatCount);

		if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
		{
			buttonCount = sizeof(GLFWgamepadstate::buttons) / sizeof(*GLFWgamepadstate::buttons);
			axisCount = sizeof(GLFWgamepadstate::axes) / sizeof(*GLFWgamepadstate::axes);
		}
		else
		{
			(void)glfwGetJoystickButtons(jid, (int*)&buttonCount);
			(void)glfwGetJoystickAxes(jid, (int*)&axisCount);
		}

		data.joysticks[jid].OnConnect(buttonCount, axisCount, hatCount);
		JoystickConnectEvent event(jid, true);
		Application::EventCallback(event);
	}

	void Input::OnJoystickDisconnected(int jid)
	{
		data.joysticks[jid].OnDisconnect();
		JoystickConnectEvent event(jid, false);
		Application::EventCallback(event);
	}

	void Input::Update()
	{
		for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++)
		{
			JoystickState& joystick = data.joysticks[jid];

			if (joystick.IsConnected())
			{
				GLFWgamepadstate state;

				int buttonCount;
				int axisCount;
				int hatCount;

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

				for (int i = 0; i < buttonCount; i++)
					joystick.SetButton(static_cast<JoystickButton>(i), buttons[i]);
				for (int i = 0; i < axisCount; i++)
					joystick.SetAxis(static_cast<JoystickAxis>(i), axes[i]);
				for (int i = 0; i < hatCount; i++)
					joystick.SetHat(static_cast<JoystickHat>(i), static_cast<JoystickHatState>(hats[i]));
			}
		}
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

	bool Input::OnMouseMoveEvent(MouseMoveEvent& event)
	{
		data.lastContext = glfwGetCurrentContext();
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

	glm::vec2 Input::GetMousePos()
	{
		double x, y;
		glfwGetCursorPos(data.lastContext, &x, &y);
		return {static_cast<float>(x), static_cast<float>(y)};
	}

	float Input::GetMousePosX()
	{
		return GetMousePos().x;
	}

	float Input::GetMousePosY()
	{
		return GetMousePos().y;
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
		int jid = static_cast<int>(joystick);
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
		int jid = static_cast<int>(joystick);
		const JoystickState& joystickState = data.joysticks[jid];
		return joystickState.IsConnected() ? joystickState.GetAxis(axis) : 0.0f;
	}

	JoystickHatState Input::GetJoystickHat(Joystick joystick, JoystickHat hat)
	{
		GBC_CORE_ASSERT(joystick < Joystick::Count, "Joystick index out of bounds!");
		int jid = static_cast<int>(joystick);
		const JoystickState& joystickState = data.joysticks[jid];
		return joystickState.IsConnected() ? joystickState.GetHat(hat) : JoystickHatState::Centered;
	}
}
