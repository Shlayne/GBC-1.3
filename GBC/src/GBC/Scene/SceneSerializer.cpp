#include "gbcpch.h"
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include "GBC/Scene/Entity.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/NativeScriptComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"

// Deserialization stuff
namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.SetStyle(YAML::EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& v)
		{
			if (node.IsSequence() && node.size() == 2)
			{
				v.x = node[0].as<float>();
				v.y = node[1].as<float>();
				return true;
			}
			return false;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.SetStyle(YAML::EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& v)
		{
			if (node.IsSequence() && node.size() == 3)
			{
				v.x = node[0].as<float>();
				v.y = node[1].as<float>();
				v.z = node[2].as<float>();
				return true;
			}
			return false;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& v)
		{
			Node node;
			node.push_back(v.x);
			node.push_back(v.y);
			node.push_back(v.z);
			node.push_back(v.w);
			node.SetStyle(YAML::EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& v)
		{
			if (node.IsSequence() && node.size() == 4)
			{
				v.x = node[0].as<float>();
				v.y = node[1].as<float>();
				v.z = node[2].as<float>();
				v.w = node[3].as<float>();
				return true;
			}
			return false;
		}
	};
}

namespace gbc
{
	static std::string SceneCameraProjectionTypeToString(SceneCamera::ProjectionType projectionType)
	{
		switch (projectionType)
		{
			case SceneCamera::ProjectionType::Perspective:  return "Perspective";
			case SceneCamera::ProjectionType::Orthographic: return "Orthographic";
		}

		GBC_CORE_ASSERT("Unknown Scene Camera Projection Type!");
		return std::string();
	}

	static SceneCamera::ProjectionType SceneCameraProjectionTypeFromString(std::string_view string)
	{
		if (string == "Perspective")       return SceneCamera::ProjectionType::Perspective;
		else if (string == "Orthographic") return SceneCamera::ProjectionType::Orthographic;

		GBC_CORE_ASSERT("Unknown Scene Camera Projection Type!");
		return static_cast<SceneCamera::ProjectionType>(0);
	}

	static std::string TextureFilterModeToString(TextureFilterMode textureFilterMode)
	{
		switch (textureFilterMode)
		{
			case TextureFilterMode::Linear:  return "Linear";
			case TextureFilterMode::Nearest: return "Nearest";
		}

		GBC_CORE_ASSERT("Unknown Texture Filter Mode!");
		return std::string();
	}

	static TextureFilterMode TextureFilterModeFromString(std::string_view string)
	{
		if (string == "Linear")       return TextureFilterMode::Linear;
		else if (string == "Nearest") return TextureFilterMode::Nearest;

		GBC_CORE_ASSERT("Unknown Texture Filter Mode!");
		return static_cast<TextureFilterMode>(0);
	}

	static std::string TextureWrapModeToString(TextureWrapMode textureFilterMode)
	{
		switch (textureFilterMode)
		{
			case TextureWrapMode::ClampToEdge: return "ClampToEdge";
			case TextureWrapMode::Repeat:      return "Repeat";
		}

		GBC_CORE_ASSERT("Unknown Texture Wrap Mode!");
		return std::string();
	}

	static TextureWrapMode TextureWrapModeFromString(std::string_view string)
	{
		if (string == "ClampToEdge") return TextureWrapMode::ClampToEdge;
		else if (string == "Repeat") return TextureWrapMode::Repeat;

		GBC_CORE_ASSERT("Unknown Texture Wrap Mode!");
		return static_cast<TextureWrapMode>(0);
	}

	static std::string Rigidbody2DComponentBodyTypeToString(Rigidbody2DComponent::BodyType rigidbody2DComponentType)
	{
		switch (rigidbody2DComponentType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		GBC_CORE_ASSERT("Unknown Rigidbody 2D Component Body Type!");
		return std::string();
	}

	static Rigidbody2DComponent::BodyType Rigidbody2DComponentBodyTypeFromString(std::string_view string)
	{
		if (string == "Static")         return Rigidbody2DComponent::BodyType::Static;
		else if (string == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		else if (string == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		GBC_CORE_ASSERT("Unknown Rigidbody 2D Component Body Type!");
		return static_cast<Rigidbody2DComponent::BodyType>(0);
	}

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << YAML::EndSeq; }

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq; }

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq; }

	template<typename T, typename Func>
	static void SerializeComponent(YAML::Emitter& out, Entity entity, const char* name, Func func)
	{
		if (entity.HasComponent<T>())
		{
			out << YAML::Key << name << YAML::Value << YAML::BeginMap;
			func(entity.GetComponent<T>());
			out << YAML::EndMap;
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap
			<< YAML::Key << "Entity" << YAML::Value << "0"; // TODO: entity ID goes here

		// An entity always has these two components
		SerializeComponent<TagComponent>(out, entity, "TagComponent", [&out](TagComponent& component)
		{
			out << YAML::Key << "Tag" << YAML::Value << component.tag;
		});
		SerializeComponent<TransformComponent>(out, entity, "TransformComponent", [&out](TransformComponent& component)
		{
			out << YAML::Key << "Translation" << YAML::Value << component.translation
				<< YAML::Key << "Rotation" << YAML::Value << component.rotation
				<< YAML::Key << "Scale" << YAML::Value << component.scale;
		});

		SerializeComponent<CameraComponent>(out, entity, "CameraComponent", [&out](CameraComponent& component)
		{
			auto& camera = component.camera;
			out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap
				<< YAML::Key << "ProjectionType" << YAML::Value << SceneCameraProjectionTypeToString(camera.GetProjectionType())
				<< YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV()
				<< YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip()
				<< YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip()
				<< YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize()
				<< YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip()
				<< YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip()
				<< YAML::EndMap
				<< YAML::Key << "Primary" << YAML::Value << component.primary;
		});
		// TODO: how do this ???
		//SerializeComponent<NativeScriptComponent>(out, entity, "NativeScriptComponent", [&out](NativeScriptComponent& component) {});
		SerializeComponent<SpriteRendererComponent>(out, entity, "SpriteRendererComponent", [&out](SpriteRendererComponent& component)
		{
			out << YAML::Key << "TintColor" << YAML::Value << component.color
				<< YAML::Key << "TilingFactor" << YAML::Value << component.tilingFactor;

			// TODO: reference texture by UUID
			if (component.texture && component.texture->GetTexture())
			{
				std::string filepath = component.texture->GetTexture()->GetFilepath();
				const auto& specs = component.texture->GetSpecification();

				out << YAML::Key << "Texture" << YAML::BeginMap
						<< YAML::Key << "Filepath" << YAML::Value << filepath
						<< YAML::Key << "Specification" << YAML::BeginMap
							<< YAML::Key << "MinFilter" << YAML::Value << TextureFilterModeToString(specs.minFilter)
							<< YAML::Key << "MagFilter" << YAML::Value << TextureFilterModeToString(specs.magFilter)
							<< YAML::Key << "WrapS" << YAML::Value << TextureWrapModeToString(specs.wrapS)
							<< YAML::Key << "WrapT" << YAML::Value << TextureWrapModeToString(specs.wrapT)
						<< YAML::EndMap
					<< YAML::EndMap;
			}
		});

		SerializeComponent<BoxCollider2DComponent>(out, entity, "BoxCollider2DComponent", [&out](BoxCollider2DComponent& component)
		{
			out << YAML::Key << "Size" << YAML::Value << component.size
				<< YAML::Key << "Offset" << YAML::Value << component.offset
				<< YAML::Key << "Density" << YAML::Value << component.density
				<< YAML::Key << "Friction" << YAML::Value << component.friction
				<< YAML::Key << "Restitution" << YAML::Value << component.restitution
				<< YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;
		});

		SerializeComponent<Rigidbody2DComponent>(out, entity, "Rigidbody2DComponent", [&out](Rigidbody2DComponent& component)
		{
			out << YAML::Key << "Type" << YAML::Value << Rigidbody2DComponentBodyTypeToString(component.type)
				<< YAML::Key << "FixedRotation" << YAML::Value << component.fixedRotation;
		});

		out << YAML::EndMap;
	}

	bool SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap
			<< YAML::Key << "Scene" << YAML::Value << "Untitled"
			<< YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		// entt::registry::each iterates backwards, this doesn't
		auto view = scene->registry->view<TagComponent>();
		for (auto it = view.rbegin(); it != view.rend(); ++it)
		{
			Entity entity(*it, scene.get());
			if (entity)
				SerializeEntity(out, entity);
		}

		out << YAML::EndSeq << YAML::EndMap;

		std::ofstream file(filepath);
		if (file.is_open())
		{
			file << out.c_str();
			file.close();
			return true;
		}

		return false;
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream file(filepath);

		if (file.is_open())
		{
			YAML::Node data = YAML::Load(file);

			YAML::Node sceneNode = data["Scene"];
			if (!sceneNode)
				return false;

			std::string sceneName = sceneNode.as<std::string>();

			auto entities = data["Entities"];
			if (entities)
			{
				for (auto entityNode : entities)
				{
					uint64_t uuid = entityNode["Entity"].as<uint64_t>(); // TODO

					std::string name;
					auto tagComponent = entityNode["TagComponent"];
					if (tagComponent)
						name = tagComponent["Tag"].as<std::string>();

					Entity entity = scene->CreateEntity(name);

					// Entities always have transform components
					{
						auto transformComponentNode = entityNode["TransformComponent"];
						auto& transformComponent = entity.GetComponent<TransformComponent>();
						transformComponent.translation = transformComponentNode["Translation"].as<glm::vec3>();
						transformComponent.rotation = transformComponentNode["Rotation"].as<glm::vec3>();
						transformComponent.scale = transformComponentNode["Scale"].as<glm::vec3>();
					}

					if (auto cameraComponentNode = entityNode["CameraComponent"]; cameraComponentNode)
					{
						auto& cameraComponent = entity.AddComponent<CameraComponent>();

						auto cameraNode = cameraComponentNode["Camera"];
						auto& camera = cameraComponent.camera;
						camera.SetProjectionType(SceneCameraProjectionTypeFromString(cameraNode["ProjectionType"].as<std::string>()));
						camera.SetPerspectiveFOV(cameraNode["PerspectiveFOV"].as<float>());
						camera.SetPerspectiveNearClip(cameraNode["PerspectiveNear"].as<float>());
						camera.SetPerspectiveFarClip(cameraNode["PerspectiveFar"].as<float>());
						camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
						camera.SetOrthographicNearClip(cameraNode["OrthographicNear"].as<float>());
						camera.SetOrthographicFarClip(cameraNode["OrthographicFar"].as<float>());

						cameraComponent.primary = cameraComponentNode["Primary"].as<bool>();
					}

					if (auto spriteRendererComponentNode = entityNode["SpriteRendererComponent"]; spriteRendererComponentNode)
					{
						auto& spriteRendererComponent = entity.AddComponent<SpriteRendererComponent>();

						// TODO: this is not at all what should happen; I'm just doing this right now so
						// I can save and load a scene and have it keep the texture.
						// This requires an asset system that uses the path and/or the uuid of the texture
						// to return the correct Ref<Texture2D>
						// something like this:
						// AssetManager::GetAssetByUUID(spriteRendererComponentNode["Texture"].as<uint64_t>())
						// or maybe this:
						// Texture2D::Create(spriteRendererComponentNode["Texture"].as<uint64_t>());
						// and that would use the asset manager to get the correct Ref<Texture2D>

						spriteRendererComponent.color = spriteRendererComponentNode["TintColor"].as<glm::vec4>();
						spriteRendererComponent.tilingFactor = spriteRendererComponentNode["TilingFactor"].as<glm::vec2>();

						if (auto textureNode = spriteRendererComponentNode["Texture"]; textureNode)
						{
							auto specsNode = textureNode["Specification"];
							TextureSpecification specs;
							specs.texture = LocalTexture2D::Create(textureNode["Filepath"].as<std::string>(), 4);
							specs.minFilter = TextureFilterModeFromString(specsNode["MinFilter"].as<std::string>());
							specs.magFilter = TextureFilterModeFromString(specsNode["MagFilter"].as<std::string>());
							specs.wrapS = TextureWrapModeFromString(specsNode["WrapS"].as<std::string>());
							specs.wrapT = TextureWrapModeFromString(specsNode["WrapT"].as<std::string>());
							spriteRendererComponent.texture = Texture2D::Create(specs);
						}
					}

					if (auto boxCollider2DComponentNode = entityNode["BoxCollider2DComponent"]; boxCollider2DComponentNode)
					{
						auto& boxCollider2DComponent = entity.AddComponent<BoxCollider2DComponent>();
						boxCollider2DComponent.size = boxCollider2DComponentNode["Size"].as<glm::vec2>();
						boxCollider2DComponent.offset = boxCollider2DComponentNode["Offset"].as<glm::vec2>();
						boxCollider2DComponent.density = boxCollider2DComponentNode["Density"].as<float>();
						boxCollider2DComponent.friction = boxCollider2DComponentNode["Friction"].as<float>();
						boxCollider2DComponent.restitution = boxCollider2DComponentNode["Restitution"].as<float>();
						boxCollider2DComponent.restitutionThreshold = boxCollider2DComponentNode["RestitutionThreshold"].as<float>();
					}

					if (auto rigidbody2DComponentNode = entityNode["Rigidbody2DComponent"]; rigidbody2DComponentNode)
					{
						auto& rigidbody2DComponent = entity.AddComponent<Rigidbody2DComponent>();
						rigidbody2DComponent.type = Rigidbody2DComponentBodyTypeFromString(rigidbody2DComponentNode["Type"].as<std::string>());
						rigidbody2DComponent.fixedRotation = rigidbody2DComponentNode["FixedRotation"].as<bool>();
					}
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	static std::ostream& operator<<(std::ostream& out, const glm::vec2& value) { return out << value.x << value.y; }
	static std::istream& operator>>(std::istream& in,        glm::vec2& value) { return in  >> value.x >> value.y; }
	static std::ostream& operator<<(std::ostream& out, const glm::vec3& value) { return out << value.x << value.y << value.z; }
	static std::istream& operator>>(std::istream& in,        glm::vec3& value) { return in  >> value.x >> value.y >> value.z; }
	static std::ostream& operator<<(std::ostream& out, const glm::vec4& value) { return out << value.x << value.y << value.z << value.w; }
	static std::istream& operator>>(std::istream& in,        glm::vec4& value) { return in  >> value.x >> value.y >> value.z >> value.w; }

	template<typename T, typename Func>
	static void SerializeComponent(std::ofstream& out, Entity entity, Func func)
	{
		if (entity.HasComponent<T>())
		{
			// TODO: Figure out if this component ID system is ok or not.
			out << typeid(T).hash_code();
			func(entity.GetComponent<T>());
		}
	}

	static void SerializeEntity(std::ofstream& out, Entity entity)
	{
		// TODO: entity ID goes here
		uint64_t uuid = 0;
		out << uuid;
		
		// An entity always has these two components
		TagComponent tagComponent = entity.GetComponent<TagComponent>();
		out << tagComponent.tag.size() << tagComponent.tag;
		TransformComponent transformComponent = entity.GetComponent<TransformComponent>();
		out << transformComponent.translation << transformComponent.rotation << transformComponent.scale;

		SerializeComponent<CameraComponent>(out, entity, [&out](CameraComponent& component)
		{
			auto& camera = component.camera;
			out << static_cast<int32_t>(camera.GetProjectionType()) << camera.GetPerspectiveFOV()
				<< camera.GetPerspectiveNearClip() << camera.GetPerspectiveFarClip()
				<< camera.GetOrthographicSize() << camera.GetOrthographicNearClip()
				<< camera.GetOrthographicFarClip() << component.primary;
		});
		// TODO: how do this ???
		//SerializeComponent<NativeScriptComponent>(out, entity, [&out](NativeScriptComponent& component) {});
		SerializeComponent<SpriteRendererComponent>(out, entity, [&out](SpriteRendererComponent& component)
		{
			// TODO: reference texture by UUID
			std::string filepath;
			if (component.texture && component.texture->GetTexture())
				filepath = component.texture->GetTexture()->GetFilepath();
			const auto& specs = component.texture->GetSpecification();

			out << component.color << filepath.size() << filepath << component.tilingFactor
				<< static_cast<int32_t>(specs.minFilter) << static_cast<int32_t>(specs.magFilter)
				<< static_cast<int32_t>(specs.wrapS) << static_cast<int32_t>(specs.wrapT);
		});

		uint64_t endOfEntity = 0;
		out << endOfEntity;
	}

	bool SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		std::ofstream file = std::ofstream(filepath, std::ios::out | std::ios::binary);

		if (file.is_open())
		{
			static constexpr char sceneName[] = "Untitled";
			static constexpr size_t sceneNameLength = sizeof(sceneName) / sizeof(*sceneName);
			size_t entityCount = scene->registry->size();
			file << sceneNameLength << sceneName << entityCount;

			auto view = scene->registry->view<TagComponent>();
			for (auto it = view.rbegin(); it != view.rend(); ++it)
			{
				Entity entity(*it, scene.get());
				if (entity)
					SerializeEntity(file, entity);
			}

			file.close();
			return true;
		}

		return false;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// TODO: implement
		return false;
	}
}
