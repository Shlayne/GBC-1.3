#include "gbcpch.h"
#include "MeshFactory3D.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	// TODO: same story as the Assets directory
	static const std::filesystem::path defaultMeshSourceDirectory = L"Meshes/Source/Default";

	Ref<Model3D> MeshFactory3D::CreateCapsule()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Capsule.fbx"); }

	Ref<Model3D> MeshFactory3D::CreateCone()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Cone.fbx"); }

	Ref<Model3D> MeshFactory3D::CreateCube()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Cube.fbx"); }

	Ref<Model3D> MeshFactory3D::CreateCylinder()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Cylinder.fbx"); }

	Ref<Model3D> MeshFactory3D::CreatePlane()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Plane.fbx"); }

	Ref<Model3D> MeshFactory3D::CreateSphere()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Sphere.fbx"); }

	Ref<Model3D> MeshFactory3D::CreateTorus()
	{ return Application::Get().GetAssetManager().GetOrLoad<Model3D>(defaultMeshSourceDirectory / "Torus.fbx"); }
}
