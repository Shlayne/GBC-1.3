#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Core/UUID.h"
#include "GBC/Scene/SceneCamera.h"
#include "GBC/Rendering/Texture.h"
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace gbc
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value);
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value);
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value);

	std::string SceneCameraProjectionTypeToString(SceneCamera::ProjectionType projectionType);
	SceneCamera::ProjectionType SceneCameraProjectionTypeFromString(std::string_view string);
	std::string TextureFilterModeToString(TextureFilterMode textureFilterMode);
	TextureFilterMode TextureFilterModeFromString(std::string_view string);
	std::string TextureWrapModeToString(TextureWrapMode textureFilterMode);
	TextureWrapMode TextureWrapModeFromString(std::string_view string);
}

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& value);
		static bool decode(const Node& node, glm::vec2& value);
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& value);
		static bool decode(const Node& node, glm::vec3& value);
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& value);
		static bool decode(const Node& node, glm::vec4& value);
	};
}
