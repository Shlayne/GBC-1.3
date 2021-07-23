#include "gbcpch.h"
#include "GBC/Core/Input.h"
#include "glfw/glfw3.h"
#include <array>
#include "GBC/Core/Application.h"

namespace gbc
{
	struct InputData
	{
		std::array<JoystickState, GLFW_JOYSTICK_LAST + 1> joysticks;
	};
	static InputData data;

	void Input::PreInit()
	{
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
			buttonCount = sizeof(GLFWgamepadstate::buttons) / sizeof(unsigned char);
			axisCount = sizeof(GLFWgamepadstate::axes) / sizeof(float);
		}
		else
		{
			(void)glfwGetJoystickButtons(jid, (int*)&buttonCount);
			(void)glfwGetJoystickAxes(jid, (int*)&axisCount);
		}

		data.joysticks[jid].OnConnect(buttonCount, axisCount, hatCount);
		JoystickConnectEvent event(jid, true);
		Application::StaticOnEvent(event);
	}

	void Input::OnJoystickDisconnected(int jid)
	{
		data.joysticks[jid].OnDisconnect();
		JoystickConnectEvent event(jid, false);
		Application::StaticOnEvent(event);
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
					buttonCount = sizeof(state.buttons) / sizeof(unsigned char);
					axisCount = sizeof(state.axes) / sizeof(float);
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

	// Key

	bool Input::IsKeyPressed(Keycode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) != GLFW_RELEASE;
	}

	bool Input::IsKeyReleased(Keycode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetKey(window, static_cast<int>(keycode)) == GLFW_RELEASE;
	}

	// Mouse

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) != GLFW_RELEASE;
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return {(float)x, (float)y};
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
		GBC_CORE_ASSERT(joystick < Joystick::Count, "Joystick state index out of bounds!");
		return data.joysticks[static_cast<size_t>(joystick)];
	}

	static bool IsJoystickButtonInState(Joystick joystick, JoystickButton button, unsigned char buttonState)
	{
		int jid = static_cast<int>(joystick);

		if (glfwJoystickPresent(jid) == GLFW_TRUE)
		{
			GBC_CORE_ASSERT(button < JoystickButton::Count);
			size_t buttonIndex = static_cast<size_t>(button);

			if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
			{
				GLFWgamepadstate state;
				glfwGetGamepadState(jid, &state);
				return state.buttons[buttonIndex] == buttonState;
			}
			else
			{
				int buttonCount;
				const unsigned char* buttons = glfwGetJoystickButtons(jid, &buttonCount);
				return buttonCount && buttons[buttonIndex] == buttonState;
			}
		}

		return false;
	}

	bool Input::IsJoystickButtonPressed(Joystick joystick, JoystickButton button)
	{ return IsJoystickButtonInState(joystick, button, GLFW_PRESS); }

	bool Input::IsJoystickButtonReleased(Joystick joystick, JoystickButton button)
	{ return IsJoystickButtonInState(joystick, button, GLFW_RELEASE); }

	float Input::GetJoystickAxis(Joystick joystick, JoystickAxis axis)
	{
		int jid = static_cast<int>(joystick);

		if (glfwJoystickPresent(jid) == GLFW_TRUE)
		{
			GBC_CORE_ASSERT(axis < JoystickAxis::Count);
			size_t axisIndex = static_cast<size_t>(axis);

			if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
			{
				GLFWgamepadstate state;
				glfwGetGamepadState(jid, &state);
				return state.axes[axisIndex];
			}
			else
			{
				int axisCount;
				const float* axes = glfwGetJoystickAxes(jid, &axisCount);
				if (axisCount)
					return axes[axisIndex];
			}
		}

		return 0.0f;
	}

	JoystickHatState Input::GetJoystickHat(Joystick joystick, JoystickHat hat)
	{
		int jid = static_cast<int>(joystick);

		if (glfwJoystickPresent(jid) == GLFW_TRUE)
		{
			GBC_CORE_ASSERT(hat < JoystickHat::Count);
			size_t axisIndex = static_cast<size_t>(hat);

			int hatCount;
			const unsigned char* hats = glfwGetJoystickHats(jid, &hatCount);
			if (hatCount)
				return static_cast<JoystickHatState>(hats[axisIndex]);
		}

		return JoystickHatState::Centered;
	}
}
