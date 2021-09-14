#include "gbcpch.h"
#include "Joysticks.h"

namespace gbc
{
	JoystickState::~JoystickState()
	{
		if (connected)
			OnDisconnect();
	}

	void JoystickState::OnConnect(uint32_t buttonCount, uint32_t axisCount, uint32_t hatCount)
	{
		GBC_CORE_ASSERT(!connected, "Joystick already connected!");
		GBC_CORE_ASSERT(buttonCount <= static_cast<uint32_t>(JoystickButton::Count), "Joystick has too many buttons for enum to handle.");
		GBC_CORE_ASSERT(axisCount <= static_cast<uint32_t>(JoystickAxis::Count), "Joystick has too many axes for enum to handle.");
		GBC_CORE_ASSERT(hatCount <= static_cast<uint32_t>(JoystickHat::Count), "Joystick has too many hats for enum to handle.");

		connected = true;

		this->buttonCount = buttonCount;
		this->axisCount = axisCount;
		this->hatCount = hatCount;

		buttons = buttonCount ? new uint8_t[1 + (buttonCount - 1) / 8]() : nullptr;
		axes = axisCount ? new float[axisCount]() : nullptr;
		hats = hatCount ? new JoystickHatState[1 + (hatCount - 1) / 2]() : nullptr;
	}

	void JoystickState::OnDisconnect()
	{
		GBC_CORE_ASSERT(connected, "Joystick already disconnected!");

		connected = false;

		buttonCount = 0;
		axisCount = 0;
		hatCount = 0;

		delete[] buttons;
		delete[] axes;
		delete[] hats;
	}

	bool JoystickState::GetButton(JoystickButton button) const
	{
		GBC_CORE_ASSERT(static_cast<uint32_t>(button) < buttonCount, "Joystick button index out of bounds!");

		uint8_t buttonBits = static_cast<uint8_t>(button);
		return buttons[buttonBits / 8] & (1 << (buttonBits % 8));
	}

	float JoystickState::GetAxis(JoystickAxis axis) const
	{
		GBC_CORE_ASSERT(static_cast<uint32_t>(axis) < axisCount, "Joystick axis index out of bounds!");
		return axes[static_cast<size_t>(axis)];
	}

	JoystickHatState JoystickState::GetHat(JoystickHat hat) const
	{
		GBC_CORE_ASSERT(static_cast<uint32_t>(hat) < hatCount, "Joystick hat index out of bounds!");

		uint8_t hatBits = static_cast<uint8_t>(hat);
		return static_cast<JoystickHatState>(static_cast<uint8_t>(hats[hatBits / 2]) & (0xf << (hatBits % 2)));
	}

	void JoystickState::SetButton(JoystickButton button, bool value)
	{
		GBC_CORE_ASSERT(static_cast<uint32_t>(button) < buttonCount, "Joystick button index out of bounds!");

		uint8_t buttonBits = static_cast<uint8_t>(button);
		uint8_t bit = (1 << (buttonBits % 8));
		if (value)
			buttons[buttonBits / 8] |= bit;
		else
			buttons[buttonBits / 8] &= ~bit;
	}

	void JoystickState::SetAxis(JoystickAxis axis, float value)
	{
		GBC_CORE_ASSERT(static_cast<uint32_t>(axis) < axisCount, "Joystick axis index out of bounds!");
		axes[static_cast<size_t>(axis)] = value;
	}

	void JoystickState::SetHat(JoystickHat hat, JoystickHatState value)
	{
		GBC_CORE_ASSERT(static_cast<uint32_t>(hat) < hatCount, "Joystick hat index out of bounds!");

		uint8_t hatBits = static_cast<uint8_t>(hat);
		uint8_t bits = 4 * (hatBits % 2);
		uint8_t newValue = (static_cast<uint8_t>(hats[hatBits / 2]) & ~(0xf << bits)) | (static_cast<uint8_t>(value) << bits);
		hats[hatBits / 2] = static_cast<JoystickHatState>(newValue);
	}
}
