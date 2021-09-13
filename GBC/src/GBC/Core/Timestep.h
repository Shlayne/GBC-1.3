#pragma once

namespace gbc
{
	class Timestep
	{
	public:
		constexpr Timestep(float seconds = 0.0f) noexcept : seconds(seconds) {}
		constexpr Timestep& operator=(float seconds) noexcept { this->seconds = seconds; return *this; }
		constexpr Timestep& operator=(Timestep timestep) noexcept { seconds = timestep.seconds; return *this; }
		constexpr operator float() const noexcept { return seconds; }

		constexpr float Seconds() const noexcept { return seconds; }
		constexpr float Millis() const noexcept { return seconds * 1000.0f; }
		constexpr float Micros() const noexcept { return seconds * 1000000.0f; }
		constexpr float Nanos() const noexcept { return seconds * 1000000000.0f; }

		constexpr Timestep operator+(Timestep timestep) const noexcept { return seconds + timestep.seconds; }
		constexpr Timestep& operator+=(Timestep timestep) noexcept { seconds += timestep.seconds; return *this; }
		constexpr Timestep operator-(Timestep timestep) const noexcept { return seconds - timestep.seconds; }
		constexpr Timestep& operator-=(Timestep timestep) noexcept { seconds -= timestep.seconds; return *this; }
		constexpr Timestep operator*(Timestep timestep) const noexcept { return seconds * timestep.seconds; }
		constexpr Timestep& operator*=(Timestep timestep) noexcept { seconds *= timestep.seconds; return *this; }
		constexpr Timestep operator/(Timestep timestep) const { return seconds / timestep.seconds; }
		constexpr Timestep& operator/=(Timestep timestep) { seconds /= timestep.seconds; return *this; }
	private:
		float seconds;
	};
}
