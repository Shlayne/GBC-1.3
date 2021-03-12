#pragma once

#include "entt/entt.hpp"
#include "Entity.h"
#include "GBC/Core/Timestep.h"

namespace gbc
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity();

		void OnUpdate(Timestep timestep);
		void OnRender();
	private:
		entt::registry registry;

		friend class Entity;
	};
}
