#pragma once

namespace gbc
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: time(time) {}

		inline operator float() const { return time; }

		inline float Seconds() const { return time; }
		inline float Millis() const { return time * 1000.0f; }
		inline float Micros() const { return time * 1000000.0f; }
		inline float Nanos() const { return time * 1000000000.0f; }

		inline Timestep operator-(Timestep timestep) { return time - timestep.time; }
		inline Timestep operator+(Timestep timestep) { return time + timestep.time; }
	private:
		float time;
	};
}
