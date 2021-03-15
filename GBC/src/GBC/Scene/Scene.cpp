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
		entity.Add<TransformComponent>();
		entity.Add<TagComponent>(tag.empty() ? "Unknown Entity" : tag);
		return entity;
	}

	void Scene::RemoveEntity(Entity entity)
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

	void Scene::OnUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		if (shouldResizeCameras)
		{
			GBC_PROFILE_SCOPE("Resize Cameras - Scene::OnUpdate(Timestep)");

			shouldResizeCameras = false;

			auto view = registry.view<CameraComponent>();
			for (auto entity : view)
			{
				auto& camera = view.get<CameraComponent>(entity);
				if (!camera.fixedAspectRatio)
					camera.camera.OnViewportResize(viewportSize.x, viewportSize.y);
			}
		}

		{
			GBC_PROFILE_SCOPE("Update Native Script Components - Scene::OnUpdate(Timestep)");

			registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc)
			{
				nsc.instance->OnUpdate(timestep);
			});
		}
	}

	void Scene::OnRender()
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

	void Scene::OnViewportResize(int width, int height)
	{
		if (width > 0 && height > 0)
		{
			shouldResizeCameras = true;
			viewportSize.x = width;
			viewportSize.y = height;
		}
	}
}
