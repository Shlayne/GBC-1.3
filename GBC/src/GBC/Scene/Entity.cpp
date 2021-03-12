#include "gbcpch.h"
#include "Entity.h"

namespace gbc
{
	Entity::Entity(entt::entity handle, Scene* context)
		: handle(handle), context(context) {}
}
