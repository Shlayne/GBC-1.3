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

#pragma region yaml_deserialization_stuff
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
#pragma endregion

namespace gbc
{
#pragma region enum_converters
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
#pragma endregion

#pragma region serialization
	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << YAML::EndSeq; }

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq; }

	static YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq; }

	template<typename T, typename Func>
	static void SerializeComponent(YAML::Emitter& out, Entity entity, const char* name, Func func)
	{
		if (entity.Has<T>())
		{
			out << YAML::Key << name << YAML::Value << YAML::BeginMap;
			func(out, entity.Get<T>());
			out << YAML::EndMap;
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap
			<< YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		// An entity always has these two components in addition to a UUID
		SerializeComponent<TagComponent>(out, entity, "TagComponent", [](YAML::Emitter& out, TagComponent& component)
		{
			out << YAML::Key << "Tag" << YAML::Value << component.tag;
		});
		SerializeComponent<TransformComponent>(out, entity, "TransformComponent", [](YAML::Emitter& out, TransformComponent& component)
		{
			out << YAML::Key << "Translation" << YAML::Value << component.translation
				<< YAML::Key << "Rotation" << YAML::Value << component.rotation
				<< YAML::Key << "Scale" << YAML::Value << component.scale;
		});

		// Optional components
		SerializeComponent<CameraComponent>(out, entity, "CameraComponent", [](YAML::Emitter& out, CameraComponent& component)
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
		//SerializeComponent<NativeScriptComponent>(out, entity, "NativeScriptComponent", [](YAML::Emitter& out, NativeScriptComponent& component) {});
		if (entity.HasRelationship())
		{
			SerializeComponent<RelationshipComponent>(out, entity, "RelationshipComponent", [entity](YAML::Emitter& out, RelationshipComponent& component)
			{
				if (entity.HasParent())
					out << YAML::Key << "Parent" << YAML::Value << entity.GetParent().GetUUID();
				if (entity.HasChildren())
				{
					out << YAML::Key << "Children" << YAML::BeginSeq;
					for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
						out << YAML::Value << child.GetUUID();
					out << YAML::EndSeq;
				}
			});
		}
		SerializeComponent<SpriteRendererComponent>(out, entity, "SpriteRendererComponent", [](YAML::Emitter& out, SpriteRendererComponent& component)
		{
			out << YAML::Key << "TintColor" << YAML::Value << component.color
				<< YAML::Key << "TilingFactor" << YAML::Value << component.tilingFactor;

			// TODO: reference texture by UUID once an asset system is in place
			if (component.texture && component.texture->GetTexture())
			{
				std::string filepath = component.texture->GetTexture()->GetFilepath().string();
				const auto& specs = component.texture->GetSpecification();

				out << YAML::Key << "Texture" << YAML::BeginMap
						<< YAML::Key << "Filepath" << YAML::Value << filepath.c_str()
						<< YAML::Key << "Specification" << YAML::BeginMap
							<< YAML::Key << "MinFilter" << YAML::Value << TextureFilterModeToString(specs.minFilter)
							<< YAML::Key << "MagFilter" << YAML::Value << TextureFilterModeToString(specs.magFilter)
							<< YAML::Key << "WrapS" << YAML::Value << TextureWrapModeToString(specs.wrapS)
							<< YAML::Key << "WrapT" << YAML::Value << TextureWrapModeToString(specs.wrapT)
						<< YAML::EndMap
					<< YAML::EndMap;
			}
		});

		SerializeComponent<BoxCollider2DComponent>(out, entity, "BoxCollider2DComponent", [](YAML::Emitter& out, BoxCollider2DComponent& component)
		{
			out << YAML::Key << "Size" << YAML::Value << component.size
				<< YAML::Key << "Offset" << YAML::Value << component.offset
				<< YAML::Key << "Density" << YAML::Value << component.density
				<< YAML::Key << "Friction" << YAML::Value << component.friction
				<< YAML::Key << "Restitution" << YAML::Value << component.restitution
				<< YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;
		});
		SerializeComponent<Rigidbody2DComponent>(out, entity, "Rigidbody2DComponent", [](YAML::Emitter& out, Rigidbody2DComponent& component)
		{
			out << YAML::Key << "Type" << YAML::Value << Rigidbody2DComponentBodyTypeToString(component.bodyType)
				<< YAML::Key << "FixedRotation" << YAML::Value << component.fixedRotation;
		});

		out << YAML::EndMap;

		// Serialize all child entities and their children
		for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
			SerializeEntity(out, child);
	}

	bool SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap
			<< YAML::Key << "Scene" << YAML::Value << "Untitled" // TODO: scene names, projects, etc.
			<< YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto handle : scene->entities)
		{
			Entity entity{ handle, scene.get() };
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

	bool SceneSerializer::SerializeRuntime(const std::filesystem::path& filepath)
	{
		// TODO: implement
		return false;
	}
#pragma endregion

#pragma region deserialization
	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		std::ifstream file(filepath);

		if (file.is_open())
		{
			YAML::Node data = YAML::Load(file);

			YAML::Node sceneNode = data["Scene"];
			if (!sceneNode)
			{
				file.close();
				return false;
			}

			//std::string sceneName = sceneNode.as<std::string>(); // TODO: use this when projects are implemented

			std::unordered_map<UUID, entt::entity> entityIDMappings;

			if (auto entities = data["Entities"])
			{
				// Create all entities and setup mappings for RelationshipComponent
				for (auto entityNode : entities)
				{
					uint64_t uuid = entityNode["Entity"].as<uint64_t>();

					std::string name;
					if (auto tagComponent = entityNode["TagComponent"])
						name = tagComponent["Tag"].as<std::string>();

					entityIDMappings[uuid] = scene->CreateEntity(uuid, name);
				}

				for (auto entityNode : entities)
				{
					Entity entity{ entityIDMappings[entityNode["Entity"].as<uint64_t>()], scene.get() };

					// Entities always have transform components
					{
						auto transformComponentNode = entityNode["TransformComponent"];
						auto& transformComponent = entity.Get<TransformComponent>();
						transformComponent.translation = transformComponentNode["Translation"].as<glm::vec3>();
						transformComponent.rotation = transformComponentNode["Rotation"].as<glm::vec3>();
						transformComponent.scale = transformComponentNode["Scale"].as<glm::vec3>();
					}

					if (auto cameraComponentNode = entityNode["CameraComponent"])
					{
						auto& cameraComponent = entity.Add<CameraComponent>();

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
					if (auto relationshipComponentNode = entityNode["RelationshipComponent"])
					{
						auto& relationshipComponent = entity.Get<RelationshipComponent>();
						if (auto parentNode = relationshipComponentNode["Parent"])
							relationshipComponent.parent = entityIDMappings[parentNode.as<uint64_t>()];
						if (auto childrenNode = relationshipComponentNode["Children"])
						{
							for (auto childNode : childrenNode)
							{
								Entity child{ entityIDMappings[childNode.as<uint64_t>()], scene.get() };
								child.SetParent(entity);
							}
						}
					}
					if (auto spriteRendererComponentNode = entityNode["SpriteRendererComponent"])
					{
						auto& spriteRendererComponent = entity.Add<SpriteRendererComponent>();

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

					if (auto boxCollider2DComponentNode = entityNode["BoxCollider2DComponent"])
					{
						auto& boxCollider2DComponent = entity.Add<BoxCollider2DComponent>();
						boxCollider2DComponent.size = boxCollider2DComponentNode["Size"].as<glm::vec2>();
						boxCollider2DComponent.offset = boxCollider2DComponentNode["Offset"].as<glm::vec2>();
						boxCollider2DComponent.density = boxCollider2DComponentNode["Density"].as<float>();
						boxCollider2DComponent.friction = boxCollider2DComponentNode["Friction"].as<float>();
						boxCollider2DComponent.restitution = boxCollider2DComponentNode["Restitution"].as<float>();
						boxCollider2DComponent.restitutionThreshold = boxCollider2DComponentNode["RestitutionThreshold"].as<float>();
					}
					if (auto rigidbody2DComponentNode = entityNode["Rigidbody2DComponent"])
					{
						auto& rigidbody2DComponent = entity.Add<Rigidbody2DComponent>();
						rigidbody2DComponent.bodyType = Rigidbody2DComponentBodyTypeFromString(rigidbody2DComponentNode["Type"].as<std::string>());
						rigidbody2DComponent.fixedRotation = rigidbody2DComponentNode["FixedRotation"].as<bool>();
					}
				}
			}

			file.close();
			return true;
		}

		return false;
	}

	bool SceneSerializer::DeserializeRuntime(const std::filesystem::path& filepath)
	{
		// TODO: implement
		return false;
	}
#pragma endregion
}
