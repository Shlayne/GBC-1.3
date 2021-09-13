#pragma once

#include <cstdint>

// Taken from glfw3.h and modified

namespace gbc
{
	enum class Joystick : uint8_t
	{
		J1  =  0,
		J2  =  1,
		J3  =  2,
		J4  =  3,
		J5  =  4,
		J6  =  5,
		J7  =  6,
		J8  =  7,
		J9  =  8,
		J10 =  9,
		J11 = 10,
		J12 = 11,
		J13 = 12,
		J14 = 13,
		J15 = 14,
		J16 = 15,

		Count
	};

	enum class JoystickButton : uint8_t
	{
		A            =  0, Cross    = A,
		B            =  1, Circle   = B,
		X            =  2, Square   = X,
		Y            =  3, Triangle = Y,
		LeftBumper   =  4,
		RightBumper  =  5,
		Back         =  6, Share    = Back,
		Start        =  7, Options  = Start,
		Guide        =  8, PSButton = Guide,
		LeftThumb    =  9,
		RightThumb   = 10,
		DPadUp       = 11,
		DPadRight    = 12,
		DPadDown     = 13,
		DPadLeft     = 14,

		Count
	};

	enum class JoystickAxis : uint8_t
	{
		LeftX        = 0,
		LeftY        = 1,
		RightX       = 2,
		RightY       = 3,
		LeftTrigger  = 4,
		RightTrigger = 5,

		Count
	};

	enum class JoystickHat : uint8_t
	{
		Hat1 = 0,
		Hat2 = 1,
		Hat3 = 2,
		Hat4 = 3,

		Count
	};

	enum class JoystickHatState : uint8_t
	{
		Centered  = 0,
		Up        = 1,
		Right     = 2,
		Down      = 4,
		Left      = 8,
		RightUp   = Right | Up,
		RightDown = Right | Down,
		LeftUp    = Left | Up,
		LeftDown  = Left | Down
	};

	class JoystickState
	{
	public:
		~JoystickState();
	public:
		inline bool IsConnected() const noexcept { return connected; }
		inline uint32_t GetButtonCount() const noexcept { return buttonCount; }
		inline uint32_t GetAxisCount() const noexcept { return axisCount; }
		inline uint32_t GetHatCount() const noexcept { return hatCount; }
		bool GetButton(JoystickButton button) const;
		float GetAxis(JoystickAxis axis) const;
		JoystickHatState GetHat(JoystickHat hat) const;
	private:
		void OnConnect(uint32_t buttonCount, uint32_t axisCount, uint32_t hatCount);
		void OnDisconnect();
		void SetButton(JoystickButton button, bool value);
		void SetAxis(JoystickAxis axis, float value);
		void SetHat(JoystickHat hat, JoystickHatState value);
		friend class Input;
	private:
		bool connected = false;
		uint32_t buttonCount = 0;
		uint32_t axisCount = 0;
		uint32_t hatCount = 0;
		uint8_t* buttons;
		float* axes;
		JoystickHatState* hats;
	};
}
