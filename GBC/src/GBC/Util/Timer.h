#pragma once

#include <chrono>
#include "GBC/Core/Timestep.h"

namespace gbc
{
	class Timer
	{
	public:
		Timer();
	public:
		void Reset();
		Timestep GetElapsedTime() const;
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}
