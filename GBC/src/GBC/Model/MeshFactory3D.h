#pragma once

#include "GBC/Core/Core.h"
#include "GBC/Model/Model3D.h"

namespace gbc
{
	class MeshFactory3D
	{
	public:
		static Ref<Model3D> CreateCapsule();
		static Ref<Model3D> CreateCone();
		static Ref<Model3D> CreateCube();
		static Ref<Model3D> CreateCylinder();
		static Ref<Model3D> CreatePlane();
		static Ref<Model3D> CreateSphere();
		static Ref<Model3D> CreateTorus();
	private:
		MeshFactory3D() = delete;
		MeshFactory3D(const MeshFactory3D&) = delete;
		MeshFactory3D(MeshFactory3D&&) = delete;
		MeshFactory3D& operator=(const MeshFactory3D&) = delete;
		MeshFactory3D& operator=(MeshFactory3D&&) = delete;
		~MeshFactory3D() = delete;
	};
}
