#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Model/Mesh3D.h"

namespace gbc
{
	class MeshFactory3D
	{
	public:
		static Ref<Mesh3D> CreateCuboid(const glm::vec3& size = glm::vec3(1.0f));
		static Ref<Mesh3D> CreateEllipsoid(uint32_t subdivisions = 4, const glm::vec3& size = glm::vec3(1.0f));
		static Ref<Mesh3D> CreatePlane(const glm::uvec2& subdivisions = glm::ivec2(1), const glm::vec2& size = glm::vec2(1.0f));
		static Ref<Mesh3D> CreateFromID(std::wstring_view id);
	private:
		MeshFactory3D() = delete;
		MeshFactory3D(const MeshFactory3D&) = delete;
		MeshFactory3D(MeshFactory3D&&) = delete;
		MeshFactory3D& operator=(const MeshFactory3D&) = delete;
		MeshFactory3D& operator=(MeshFactory3D&&) = delete;
		~MeshFactory3D() = delete;
	};
}
