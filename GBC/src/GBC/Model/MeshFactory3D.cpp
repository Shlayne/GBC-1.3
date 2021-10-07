#include "gbcpch.h"
#include "MeshFactory3D.h"
#include "GBC/Rendering/RendererAPI.h"

namespace gbc
{
	static constexpr std::wstring_view cuboidID = L"GBC:Cube";
	static constexpr std::wstring_view ellipsoidID = L"GBC:Ellipse";
	static constexpr std::wstring_view planeID = L"GBC:Plane";

	Ref<Mesh3D> MeshFactory3D::CreateCuboid(const glm::vec3& size)
	{
		Ref<Mesh3D> mesh = CreateRef<Mesh3D>();
		auto& vertices = mesh->vertices;
		auto& indices = mesh->indices;
		mesh->filepath = cuboidID;

		auto AddFaceIndices = [&vertices, &indices]()
		{
			uint32_t vertexStart = static_cast<uint32_t>(vertices.size());
			indices.push_back(vertexStart + 0);
			indices.push_back(vertexStart + 1);
			indices.push_back(vertexStart + 3);
			indices.push_back(vertexStart + 3);
			indices.push_back(vertexStart + 2);
			indices.push_back(vertexStart + 0);
		};

		glm::vec3 halfSize = glm::abs(size) * 0.5f;
		glm::vec3 positionLeftBottomBack  { -halfSize.x, -halfSize.y, -halfSize.z };
		glm::vec3 positionRightBottomBack {  halfSize.x, -halfSize.y, -halfSize.z };
		glm::vec3 positionLeftTopBack     { -halfSize.x,  halfSize.y, -halfSize.z };
		glm::vec3 positionRightTopBack    {  halfSize.x,  halfSize.y, -halfSize.z };
		glm::vec3 positionLeftBottomFront { -halfSize.x, -halfSize.y,  halfSize.z };
		glm::vec3 positionRightBottomFront{  halfSize.x, -halfSize.y,  halfSize.z };
		glm::vec3 positionLeftTopFront    { -halfSize.x,  halfSize.y,  halfSize.z };
		glm::vec3 positionRightTopFront   {  halfSize.x,  halfSize.y,  halfSize.z };

		static constexpr glm::vec3 normalLeft  { -1.0f,  0.0f,  0.0f };
		static constexpr glm::vec3 normalRight {  1.0f,  0.0f,  0.0f };
		static constexpr glm::vec3 normalBottom{  0.0f, -1.0f,  0.0f };
		static constexpr glm::vec3 normalTop   {  0.0f,  1.0f,  0.0f };
		static constexpr glm::vec3 normalBack  {  0.0f,  0.0f, -1.0f };
		static constexpr glm::vec3 normalFront {  0.0f,  0.0f,  1.0f };

		glm::vec2 texCoordLeftBottom { 0.0f, 1.0f };
		glm::vec2 texCoordRightBottom{ 1.0f, 1.0f };
		glm::vec2 texCoordLeftTop    { 0.0f, 0.0f };
		glm::vec2 texCoordRightTop   { 1.0f, 0.0f };

		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			std::swap(texCoordLeftBottom, texCoordLeftTop);
			std::swap(texCoordRightBottom, texCoordRightTop);
		}

		static constexpr glm::vec4 color{ 1.0f };

		// Left face
		AddFaceIndices();
		vertices.emplace_back(positionLeftBottomBack,   normalLeft,   color, texCoordLeftBottom);
		vertices.emplace_back(positionLeftBottomFront,  normalLeft,   color, texCoordRightBottom);
		vertices.emplace_back(positionLeftTopBack,      normalLeft,   color, texCoordLeftTop);
		vertices.emplace_back(positionLeftTopFront,     normalLeft,   color, texCoordRightTop);

		// Right face
		AddFaceIndices();
		vertices.emplace_back(positionRightBottomFront, normalRight,  color, texCoordLeftBottom);
		vertices.emplace_back(positionRightBottomBack,  normalRight,  color, texCoordRightBottom);
		vertices.emplace_back(positionRightTopFront,    normalRight,  color, texCoordLeftTop);
		vertices.emplace_back(positionRightTopBack,     normalRight,  color, texCoordRightTop);

		// Bottom face
		AddFaceIndices();
		vertices.emplace_back(positionLeftBottomBack,   normalBottom, color, texCoordLeftBottom);
		vertices.emplace_back(positionRightBottomBack,  normalBottom, color, texCoordRightBottom);
		vertices.emplace_back(positionLeftBottomFront,  normalBottom, color, texCoordLeftTop);
		vertices.emplace_back(positionRightBottomFront, normalBottom, color, texCoordRightTop);

		// Top face
		AddFaceIndices();
		vertices.emplace_back(positionLeftTopFront,     normalTop,    color, texCoordLeftBottom);
		vertices.emplace_back(positionRightTopFront,    normalTop,    color, texCoordRightBottom);
		vertices.emplace_back(positionLeftTopBack,      normalTop,    color, texCoordLeftTop);
		vertices.emplace_back(positionRightTopBack,     normalTop,    color, texCoordRightTop);

		// Back face
		AddFaceIndices();
		vertices.emplace_back(positionLeftBottomFront,  normalBack,   color, texCoordLeftBottom);
		vertices.emplace_back(positionRightBottomFront, normalBack,   color, texCoordRightBottom);
		vertices.emplace_back(positionLeftTopFront,     normalBack,   color, texCoordLeftTop);
		vertices.emplace_back(positionRightTopFront,    normalBack,   color, texCoordRightTop);

		// Front face
		AddFaceIndices();
		vertices.emplace_back(positionRightBottomBack,  normalFront,  color, texCoordLeftBottom);
		vertices.emplace_back(positionLeftBottomBack,   normalFront,  color, texCoordRightBottom);
		vertices.emplace_back(positionRightTopBack,     normalFront,  color, texCoordLeftTop);
		vertices.emplace_back(positionLeftTopBack,      normalFront,  color, texCoordRightTop);

		return mesh;
	}

	Ref<Mesh3D> MeshFactory3D::CreateEllipsoid(uint32_t subdivisions, const glm::vec3& size)
	{
		Ref<Mesh3D> mesh = CreateRef<Mesh3D>();
		auto& vertices = mesh->vertices;
		auto& indices = mesh->indices;
		mesh->filepath = ellipsoidID;

		glm::vec2 absSize = glm::abs(size);

		return mesh;
	}

	Ref<Mesh3D> MeshFactory3D::CreatePlane(const glm::uvec2& subdivisions, const glm::vec2& size)
	{
		Ref<Mesh3D> mesh = CreateRef<Mesh3D>();
		auto& vertices = mesh->vertices;
		auto& indices = mesh->indices;
		mesh->filepath = planeID;

		glm::vec2 absSize = glm::abs(size);

		return mesh;
	}

	Ref<Mesh3D> MeshFactory3D::CreateFromID(std::wstring_view id)
	{
		if (id == cuboidID) return CreateCuboid();
		if (id == ellipsoidID) return CreateEllipsoid();
		if (id == planeID) return CreatePlane();
		return nullptr;
	}
}
