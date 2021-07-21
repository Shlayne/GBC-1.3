#pragma once

namespace gbc
{
	class Timestep
	{
	public:
		Timestep(float seconds = 0.0f) noexcept : seconds(seconds) {}
		inline Timestep& operator=(float seconds) noexcept { this->seconds = seconds; return *this; }
		inline Timestep& operator=(Timestep timestep) noexcept { seconds = timestep.seconds; return *this; }
		inline operator float() const noexcept { return seconds; }

		inline float Seconds() const noexcept { return seconds; }
		inline float Millis() const noexcept { return seconds * 1000.0f; }
		inline float Micros() const noexcept { return seconds * 1000000.0f; }
		inline float Nanos() const noexcept { return seconds * 1000000000.0f; }

		inline Timestep operator+(Timestep timestep) noexcept { return seconds + timestep.seconds; }
		inline Timestep& operator+=(Timestep timestep) noexcept { seconds += timestep.seconds; return *this; }
		inline Timestep operator-(Timestep timestep) noexcept { return seconds - timestep.seconds; }
		inline Timestep& operator-=(Timestep timestep) noexcept { seconds -= timestep.seconds; return *this; }
		inline Timestep operator*(Timestep timestep) noexcept { return seconds * timestep.seconds; }
		inline Timestep& operator*=(Timestep timestep) noexcept { seconds *= timestep.seconds; return *this; }
		inline Timestep operator/(Timestep timestep) { return seconds / timestep.seconds; }
		inline Timestep& operator/=(Timestep timestep) { seconds /= timestep.seconds; return *this; }
	private:
		float seconds;
	};
}
