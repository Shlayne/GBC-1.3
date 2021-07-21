#include "gbcpch.h"
#include "Entity.h"

namespace gbc
{
	Entity::Entity(entt::entity handle, Scene* context) noexcept
		: handle(handle), context(context) {}
}
