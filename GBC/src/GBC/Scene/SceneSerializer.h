#pragma once

#include "Scene.h"
#include "GBC/Core/Core.h"

namespace gbc
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		// These serialize into yaml format
		bool Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);

		// These serialize into binary format
		bool SerializeRuntime(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
	private:
		Ref<Scene> scene;
	};
}
