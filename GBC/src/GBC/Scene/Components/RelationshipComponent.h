#pragma once

#include <entt/entt.hpp>

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
