#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Scene/Scene.h"

namespace gbc
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene) : scene(scene) {}

		// These serialize into yaml format
		bool Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

		// These serialize into binary format
		bool SerializeRuntime(const std::filesystem::path& filepath);
		bool DeserializeRuntime(const std::filesystem::path& filepath);
	private:
		Ref<Scene> scene;
	};
}
