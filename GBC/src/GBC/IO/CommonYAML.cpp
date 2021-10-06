#include "gbcpch.h"
#include "CommonYAML.h"

namespace gbc
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << YAML::EndSeq; }

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq; }

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value)
	{ return out << YAML::Flow << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq; }

	std::string SceneCameraProjectionTypeToString(SceneCamera::ProjectionType projectionType)
	{
		switch (projectionType)
		{
			case SceneCamera::ProjectionType::Perspective:  return "Perspective";
			case SceneCamera::ProjectionType::Orthographic: return "Orthographic";
		}

		GBC_CORE_ASSERT("Unknown Scene Camera Projection Type!");
		return std::string();
	}

	SceneCamera::ProjectionType SceneCameraProjectionTypeFromString(std::string_view string)
	{
		if (string == "Perspective")       return SceneCamera::ProjectionType::Perspective;
		else if (string == "Orthographic") return SceneCamera::ProjectionType::Orthographic;

		GBC_CORE_ASSERT("Unknown Scene Camera Projection Type!");
		return static_cast<SceneCamera::ProjectionType>(0);
	}

	std::string TextureFilterModeToString(TextureFilterMode textureFilterMode)
	{
		switch (textureFilterMode)
		{
			case TextureFilterMode::Linear:  return "Linear";
			case TextureFilterMode::Nearest: return "Nearest";
		}

		GBC_CORE_ASSERT("Unknown Texture Filter Mode!");
		return std::string();
	}

	TextureFilterMode TextureFilterModeFromString(std::string_view string)
	{
		if (string == "Linear")       return TextureFilterMode::Linear;
		else if (string == "Nearest") return TextureFilterMode::Nearest;

		GBC_CORE_ASSERT("Unknown Texture Filter Mode!");
		return static_cast<TextureFilterMode>(0);
	}

	std::string TextureWrapModeToString(TextureWrapMode textureFilterMode)
	{
		switch (textureFilterMode)
		{
			case TextureWrapMode::ClampToEdge: return "ClampToEdge";
			case TextureWrapMode::Repeat:      return "Repeat";
		}

		GBC_CORE_ASSERT("Unknown Texture Wrap Mode!");
		return std::string();
	}

	TextureWrapMode TextureWrapModeFromString(std::string_view string)
	{
		if (string == "ClampToEdge") return TextureWrapMode::ClampToEdge;
		else if (string == "Repeat") return TextureWrapMode::Repeat;

		GBC_CORE_ASSERT("Unknown Texture Wrap Mode!");
		return static_cast<TextureWrapMode>(0);
	}
}

namespace YAML
{
	Node convert<glm::vec2>::encode(const glm::vec2& value)
	{
		Node node;
		node.push_back(value.x);
		node.push_back(value.y);
		node.SetStyle(YAML::EmitterStyle::Flow);
		return node;
	}

	bool convert<glm::vec2>::decode(const Node& node, glm::vec2& value)
	{
		if (node.IsSequence() && node.size() == 2)
		{
			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			return true;
		}
		return false;
	}

	Node convert<glm::vec3>::encode(const glm::vec3& value)
	{
		Node node;
		node.push_back(value.x);
		node.push_back(value.y);
		node.push_back(value.z);
		node.SetStyle(YAML::EmitterStyle::Flow);
		return node;
	}

	bool convert<glm::vec3>::decode(const Node& node, glm::vec3& value)
	{
		if (node.IsSequence() && node.size() == 3)
		{
			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			return true;
		}
		return false;
	}

	Node convert<glm::vec4>::encode(const glm::vec4& value)
	{
		Node node;
		node.push_back(value.x);
		node.push_back(value.y);
		node.push_back(value.z);
		node.push_back(value.w);
		node.SetStyle(YAML::EmitterStyle::Flow);
		return node;
	}

	bool convert<glm::vec4>::decode(const Node& node, glm::vec4& value)
	{
		if (node.IsSequence() && node.size() == 4)
		{
			value.x = node[0].as<float>();
			value.y = node[1].as<float>();
			value.z = node[2].as<float>();
			value.w = node[3].as<float>();
			return true;
		}
		return false;
	}
}
