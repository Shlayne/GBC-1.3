#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Model/Mesh3D.h"

namespace gbc
{
	class MeshFactory3D
	{
	public:
		static constexpr std::wstring_view cubeID = L"GBC:Cube";
		static constexpr std::wstring_view sphereID = L"GBC:Sphere";
		static constexpr std::wstring_view planeID = L"GBC:Plane";
	public:
		// Creates a cube
		static Ref<Mesh3D> CreateCube();
		// Creates an icosphere
		static Ref<Mesh3D> CreateSphere();
		// Creates a plane
		static Ref<Mesh3D> CreatePlane();
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
