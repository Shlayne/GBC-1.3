#include "gbcpch.h"
#include "Scene.h"
#include "GBC/Core/Application.h"
#include "GBC/Rendering/Basic/BasicRenderer.h"
#include "GBC/Rendering/Renderer.h"
#include "Components/TransformComponent.h"
#include "Components/TagComponent.h"
#include "Components/MeshComponent.h"
#include "Components/RenderableComponent.h"
#include "Components/CameraComponent.h"
#include "Components/NativeScriptComponent.h"

namespace gbc
{
	Scene::Scene()
	{
		GBC_PROFILE_FUNCTION();

		Window& window = Application::Get().GetWindow();
		viewportSize = {window.GetWidth(), window.GetHeight()};
	}

	Scene::~Scene()
	{
		GBC_PROFILE_FUNCTION();
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		GBC_PROFILE_FUNCTION();

		Entity entity(registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag.empty() ? "Unknown Entity" : tag);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		GBC_PROFILE_FUNCTION();

		registry.destroy(entity);
	}

	void Scene::OnCreate()
	{
		GBC_PROFILE_FUNCTION();

		registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance = nsc.createFunc();
			nsc.instance->entity = {entity, this};
			nsc.instance->OnCreate();
		});
	}

	void Scene::OnDestroy()
	{
		GBC_PROFILE_FUNCTION();

		registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnDestroy();
			nsc.destroyFunc(&nsc);
		});
	}

	void Scene::OnUpdateRuntime(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnUpdate(timestep);
		});
	}

	void Scene::OnRenderRuntime()
	{
		GBC_PROFILE_FUNCTION();

		Renderer::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		Renderer::Clear();

		// Get camera
		glm::mat4 primaryCameraTransform;
		Camera* primaryCamera = nullptr;
		{
			auto group = registry.group(entt::get<TransformComponent, CameraComponent>);
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary)
				{
					primaryCameraTransform = transform;
					primaryCamera = &camera.camera;
					break;
				}
			}
		}

		if (primaryCamera != nullptr)
		{
			BasicRenderer::BeginScene(*primaryCamera, primaryCameraTransform);

			auto group = registry.group(entt::get<MeshComponent, TransformComponent, RenderableComponent>);
			for (auto entity : group)
			{
				auto [mesh, transform, renderable] = group.get<MeshComponent, TransformComponent, RenderableComponent>(entity);
				BasicRenderer::Submit(mesh, transform, renderable);
			}

			BasicRenderer::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

	}

	void Scene::OnRenderEditor(EditorCamera& camera)
	{
		GBC_PROFILE_FUNCTION();

		Renderer::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		Renderer::Clear();

		BasicRenderer::BeginScene(camera);

		auto group = registry.group(entt::get<MeshComponent, TransformComponent, RenderableComponent>);
		for (auto entity : group)
		{
			auto [mesh, transform, renderable] = group.get<MeshComponent, TransformComponent, RenderableComponent>(entity);
			BasicRenderer::Submit(mesh, transform, renderable);
		}

		BasicRenderer::EndScene();
	}

	void Scene::OnViewportResize(int width, int height)
	{
		GBC_PROFILE_FUNCTION();

		if (width > 0 && height > 0)
		{
			viewportSize.x = width;
			viewportSize.y = height;

			auto view = registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				if (!camera.fixedAspectRatio)
					camera.camera.OnViewportResize(viewportSize.x, viewportSize.y);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		// TODO: primary camera entity should be stored in Scene, not in CameraComponent
		auto view = registry.view<CameraComponent>();
		for (auto handle : view)
			if (view.get<CameraComponent>(handle).primary)
				return {handle, this};
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		// TODO: this prevents adding custom user-defined components
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

	template<>
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component) {}

	template<>
	void Scene::OnComponentAdded<RenderableComponent>(Entity entity, RenderableComponent& component) {}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.OnViewportResize(viewportSize.x, viewportSize.y);
	}
}
