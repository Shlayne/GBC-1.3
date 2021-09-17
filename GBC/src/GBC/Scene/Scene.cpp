#include "gbcpch.h"
#include "Scene.h"
#include <entt/entt.hpp>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include "GBC/Core/Application.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/Basic/BasicRenderer.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/NativeScriptComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"

namespace gbc
{
	static b2BodyType Rigidbody2DTypeToB2BodyType(Rigidbody2DComponent::BodyType type)
	{
		switch (type)
		{
			case gbc::Rigidbody2DComponent::BodyType::Static:    return b2BodyType::b2_staticBody;
			case gbc::Rigidbody2DComponent::BodyType::Dynamic:   return b2BodyType::b2_dynamicBody;
			case gbc::Rigidbody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
		}

		GBC_CORE_ASSERT(false, "Unknown Rigidbody 2D Component Type!");
		return static_cast<b2BodyType>(0);
	}

	Scene::Scene()
	{
		GBC_PROFILE_FUNCTION();

		Window& window = Application::Get().GetWindow();
		viewportSize = { window.GetWidth(), window.GetHeight() };

		registry = new entt::registry();
	}

	Scene::~Scene()
	{
		GBC_PROFILE_FUNCTION();

		if (physicsWorld)
		{
			delete physicsWorld;
			physicsWorld = nullptr; // TODO: not sure if this is necessary for smart pointer usage but whatever.
		}
		delete registry;
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		GBC_PROFILE_FUNCTION();

		Entity entity(registry->create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag.empty() ? "Unknown Entity" : tag);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		GBC_PROFILE_FUNCTION();

		registry->destroy(entity);
	}

	void Scene::OnRuntimePlay()
	{
		GBC_PROFILE_FUNCTION();

		registry->view<NativeScriptComponent>().each([this](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance = nsc.createFunc();
			nsc.instance->entity = { entity, this };
			nsc.instance->OnCreate();
		});

		physicsWorld = new b2World({ 0.0f, -9.80665f });
		auto view = registry->view<Rigidbody2DComponent>();
		for (auto handle : view)
			InitializePhysicsEntityRigidbody2D({ handle, this });
	}

	void Scene::OnRuntimeStop()
	{
		GBC_PROFILE_FUNCTION();

		delete physicsWorld;
		physicsWorld = nullptr;

		registry->view<NativeScriptComponent>().each([](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnDestroy();
			nsc.destroyFunc(&nsc);
		});
	}

	void Scene::OnRuntimeUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		// Update scripts
		registry->view<NativeScriptComponent>().each([timestep = timestep](entt::entity entity, NativeScriptComponent& nsc)
		{
			nsc.instance->OnUpdate(timestep);
		});

		// Update physics
		{
			int32_t velocityIterations = 6;
			int32_t positionIterations = 2;
			physicsWorld->Step(timestep, velocityIterations, positionIterations);

			auto view = registry->view<Rigidbody2DComponent>();
			for (auto handle : view)
			{
				Entity entity = { handle, this };
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				Rigidbody2DComponent& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

				b2Body* body = static_cast<b2Body*>(rigidbody.runtimeBody);
				const auto& position = body->GetPosition();
				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation.z = body->GetAngle();
			}
		}
	}

	void Scene::OnRuntimeRender()
	{
		GBC_PROFILE_FUNCTION();

		// Get camera
		glm::mat4 primaryCameraTransform;
		Camera* primaryCamera = nullptr;
		{
			auto group = registry->group(entt::get<TransformComponent, CameraComponent>);
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
			BasicRenderer::BeginScene(*primaryCamera, glm::inverse(primaryCameraTransform));

			auto group = registry->group(entt::get<TransformComponent, SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, renderable] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				BasicRenderer::DrawQuad(transform, renderable);
			}

			BasicRenderer::EndScene();
		}
	}

	void Scene::OnEditorUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();
	}

	void Scene::OnEditorRender(const EditorCamera& camera)
	{
		GBC_PROFILE_FUNCTION();

		BasicRenderer::BeginScene(camera);

		auto group = registry->group(entt::get<TransformComponent, SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, renderable] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			BasicRenderer::DrawQuad(transform, renderable);
		}

		BasicRenderer::EndScene();
	}

	void Scene::OnViewportResize(int32_t width, int32_t height)
	{
		GBC_PROFILE_FUNCTION();

		if (width > 0 && height > 0)
		{
			viewportSize.x = width;
			viewportSize.y = height;

			auto view = registry->view<CameraComponent>();
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
		auto view = registry->view<CameraComponent>();
		for (auto handle : view)
			if (view.get<CameraComponent>(handle).primary)
				return { handle, this };
		return {};
	}

	void Scene::InitializePhysicsEntityRigidbody2D(Entity entity)
	{
		TransformComponent& transform = entity.GetComponent<TransformComponent>();
		Rigidbody2DComponent& rigidbody = entity.GetComponent<Rigidbody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Rigidbody2DTypeToB2BodyType(rigidbody.type);
		bodyDef.position.Set(transform.translation.x, transform.translation.y);
		bodyDef.angle = transform.rotation.z;

		b2Body* body = physicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rigidbody.fixedRotation);
		rigidbody.runtimeBody = body;

		if (entity.HasComponent<BoxCollider2DComponent>())
			InitializePhysicsEntityBoxCollider2D(entity, transform, rigidbody);
	}

	void Scene::InitializePhysicsEntityBoxCollider2D(Entity entity, TransformComponent& transform, Rigidbody2DComponent& rigidbody)
	{
		BoxCollider2DComponent& collider = entity.GetComponent<BoxCollider2DComponent>();
		if (collider.runtimeFixture == nullptr)
		{
			b2PolygonShape shape;
			glm::vec2 scale = transform.scale; // converts from glm::vec3 to glm::vec2 only once
			glm::vec2 size = scale * collider.size;
			glm::vec2 offset = scale * collider.offset;
			shape.SetAsBox(size.x, size.y, { offset.x, offset.y }, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = collider.density;
			fixtureDef.friction = collider.friction;
			fixtureDef.restitution = collider.restitution;
			fixtureDef.restitutionThreshold = collider.restitutionThreshold;
			static_cast<b2Body*>(rigidbody.runtimeBody)->CreateFixture(&fixtureDef);
		}
	}

	// OnComponentAdded
	template<typename T> void Scene::OnComponentAdded(Entity entity, T& component) { static_assert(false); }

	template<> void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.OnViewportResize(viewportSize.x, viewportSize.y);
	}

	template<> void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent & component) {}
	template<> void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent & component) {}
	template<> void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent & component) {}
	template<> void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent & component) {}

	// OnComponentAdded::Physics
	template<> void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		if (physicsWorld && entity.HasComponent<Rigidbody2DComponent>())
		{
			TransformComponent& transform = entity.GetComponent<TransformComponent>();
			Rigidbody2DComponent& rigidbody = entity.GetComponent<Rigidbody2DComponent>();
			InitializePhysicsEntityBoxCollider2D(entity, transform, rigidbody);
		}
	}

	template<> void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
		if (physicsWorld)
			InitializePhysicsEntityRigidbody2D(entity);
	}

	// OnComponentRemoved
	template<typename T> void Scene::OnComponentRemoved(Entity entity, T& component) { static_assert(false); }

	template<> void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component) {}
	template<> void Scene::OnComponentRemoved<TagComponent>(Entity entity, TagComponent& component) { GBC_CORE_ASSERT(false); }
	template<> void Scene::OnComponentRemoved<TransformComponent>(Entity entity, TransformComponent& component) { GBC_CORE_ASSERT(false); }
	template<> void Scene::OnComponentRemoved<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}
	template<> void Scene::OnComponentRemoved<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	// OnComponentAdded::Physics
	template<> void Scene::OnComponentRemoved<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		
	}
	template<> void Scene::OnComponentRemoved<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
		
	}
}
