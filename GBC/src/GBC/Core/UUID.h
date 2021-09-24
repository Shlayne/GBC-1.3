#pragma once

#include <cstdint>
#include <xhash>

namespace gbc
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;
		inline operator uint64_t() const noexcept { return uuid; }
	private:
		uint64_t uuid;
	};
}

namespace std
{
	template<>
	struct hash<gbc::UUID>
	{
		size_t operator()(const gbc::UUID& uuid) const noexcept
		{
			return hash<uint64_t>()(uuid);
		}
	};
}
