#pragma once

#include <string>
#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include "GBC/Core/Timestep.h"
#include "GBC/Events/Event.h"
#include "GBC/Events/WindowEvents.h"

namespace gbc
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& tag = std::string());
		void DestroyEntity(Entity entity);

		void OnCreate();
		void OnDestroy();
		void OnUpdate(Timestep timestep);
		void OnRender();

		void OnViewportResize(int width, int height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry registry;

		bool shouldResizeCameras = true;
		glm::ivec2 viewportSize;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}
