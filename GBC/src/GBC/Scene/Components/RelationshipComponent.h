#pragma once

// NOTE: Include this AFTER you have included entt.hpp

// This is here as a last resort fallback option
//#include <cstdint>
//namespace entt
//{
//	using id_type = std::uint32_t;
//	enum class entity : id_type;
//	static constexpr entity gbc_null = static_cast<entt::entity>(0xFFFFF); // entt::null
//}

namespace gbc
{
	// Unconstrained model: https://skypjack.github.io/2019-06-25-ecs-baf-part-4/
	struct RelationshipComponent
	{
		entt::entity head = entt::null;
		entt::entity prev = entt::null;
		entt::entity next = entt::null;
		entt::entity parent = entt::null;
	};
}
