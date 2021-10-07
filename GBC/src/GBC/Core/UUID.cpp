#include "gbcpch.h"
#include "UUID.h"
#include <random>

namespace gbc
{
	static std::random_device random;
	static std::mt19937_64 engine(random());
	static std::uniform_int_distribution<uint64_t> distribution;

	UUID::UUID() : uuid(distribution(engine)) {}
}
