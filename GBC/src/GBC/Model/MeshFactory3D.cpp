#include "gbcpch.h"
#include "MeshFactory3D.h"
#include "GBC/Rendering/RendererAPI.h"

namespace gbc
{
	std::unordered_map<std::wstring_view, Ref<Mesh3D>> meshes;

	Ref<Mesh3D> MeshFactory3D::CreateCube()
	{
		if (auto it = meshes.find(cubeID); it != meshes.end())
			return it->second;

		Ref<Mesh3D> mesh = CreateRef<Mesh3D>();
		auto& vertices = mesh->vertices;
		auto& indices = mesh->indices;

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

		static constexpr glm::vec3 positionLeftBottomBack  { -0.5f, -0.5f, -0.5f };
		static constexpr glm::vec3 positionRightBottomBack {  0.5f, -0.5f, -0.5f };
		static constexpr glm::vec3 positionLeftTopBack     { -0.5f,  0.5f, -0.5f };
		static constexpr glm::vec3 positionRightTopBack    {  0.5f,  0.5f, -0.5f };
		static constexpr glm::vec3 positionLeftBottomFront { -0.5f, -0.5f,  0.5f };
		static constexpr glm::vec3 positionRightBottomFront{  0.5f, -0.5f,  0.5f };
		static constexpr glm::vec3 positionLeftTopFront    { -0.5f,  0.5f,  0.5f };
		static constexpr glm::vec3 positionRightTopFront   {  0.5f,  0.5f,  0.5f };

		static constexpr glm::vec3 normalLeft  { -1.0f,  0.0f,  0.0f };
		static constexpr glm::vec3 normalRight {  1.0f,  0.0f,  0.0f };
		static constexpr glm::vec3 normalBottom{  0.0f, -1.0f,  0.0f };
		static constexpr glm::vec3 normalTop   {  0.0f,  1.0f,  0.0f };
		static constexpr glm::vec3 normalBack  {  0.0f,  0.0f, -1.0f };
		static constexpr glm::vec3 normalFront {  0.0f,  0.0f,  1.0f };

		glm::vec2 texCoordLeftBottom { 0.0f, 1.0f };
		glm::vec2 texCoordRightBottom{ 1.0f, 1.0f };
		glm::vec2 texCoordLeftTop	 { 0.0f, 0.0f };
		glm::vec2 texCoordRightTop	 { 1.0f, 0.0f };
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			std::swap(texCoordLeftBottom, texCoordLeftTop);
			std::swap(texCoordRightBottom, texCoordRightTop);
		}

		// Left face
		AddFaceIndices();
		vertices.emplace_back(positionLeftBottomBack,   normalLeft,  texCoordLeftBottom);
		vertices.emplace_back(positionLeftBottomFront,  normalLeft,  texCoordRightBottom);
		vertices.emplace_back(positionLeftTopBack,      normalLeft,  texCoordLeftTop);
		vertices.emplace_back(positionLeftTopFront,     normalLeft,  texCoordRightTop);
																	 
		// Right face												 
		AddFaceIndices();											 
		vertices.emplace_back(positionRightBottomFront, normalRight,  texCoordLeftBottom);
		vertices.emplace_back(positionRightBottomBack,  normalRight,  texCoordRightBottom);
		vertices.emplace_back(positionRightTopFront,    normalRight,  texCoordLeftTop);
		vertices.emplace_back(positionRightTopBack,     normalRight,  texCoordRightTop);

		// Bottom face
		AddFaceIndices();
		vertices.emplace_back(positionLeftBottomBack,   normalBottom, texCoordLeftBottom);
		vertices.emplace_back(positionRightBottomBack,  normalBottom, texCoordRightBottom);
		vertices.emplace_back(positionLeftBottomFront,  normalBottom, texCoordLeftTop);
		vertices.emplace_back(positionRightBottomFront, normalBottom, texCoordRightTop);

		// Top face
		AddFaceIndices();
		vertices.emplace_back(positionLeftTopFront,     normalTop,    texCoordLeftBottom);
		vertices.emplace_back(positionRightTopFront,    normalTop,    texCoordRightBottom);
		vertices.emplace_back(positionLeftTopBack,      normalTop,    texCoordLeftTop);
		vertices.emplace_back(positionRightTopBack,     normalTop,    texCoordRightTop);

		// Back face
		AddFaceIndices();
		vertices.emplace_back(positionLeftBottomFront,  normalBack,   texCoordLeftBottom);
		vertices.emplace_back(positionRightBottomFront, normalBack,   texCoordRightBottom);
		vertices.emplace_back(positionLeftTopFront,     normalBack,   texCoordLeftTop);
		vertices.emplace_back(positionRightTopFront,    normalBack,   texCoordRightTop);

		// Front face
		AddFaceIndices();
		vertices.emplace_back(positionRightBottomBack,  normalFront,  texCoordLeftBottom);
		vertices.emplace_back(positionLeftBottomBack,   normalFront,  texCoordRightBottom);
		vertices.emplace_back(positionRightTopBack,     normalFront,  texCoordLeftTop);
		vertices.emplace_back(positionLeftTopBack,      normalFront,  texCoordRightTop);

		meshes[cubeID] = mesh;
		return mesh;
	}

	struct Triangle
	{
		glm::vec3 position0;
		glm::vec3 position1;
		glm::vec3 position2;
	};

	static void Subdivide(std::vector<Triangle>& outTriangles, const glm::vec3& position0, const glm::vec3& position1, const glm::vec3& position2, uint32_t subdivision)
	{
		if (subdivision == 0)
		{
			outTriangles.emplace_back(position0, position1, position2);
			return;
		}

		glm::vec3 midpoint0 = glm::normalize(position0 + position1);
		glm::vec3 midpoint1 = glm::normalize(position1 + position2);
		glm::vec3 midpoint2 = glm::normalize(position2 + position0);

		subdivision--;
		Subdivide(outTriangles, position0, midpoint0, midpoint2, subdivision);
		Subdivide(outTriangles, position1, midpoint1, midpoint0, subdivision);
		Subdivide(outTriangles, position2, midpoint2, midpoint1, subdivision);
		Subdivide(outTriangles, midpoint0, midpoint1, midpoint2, subdivision);
	}

	Ref<Mesh3D> MeshFactory3D::CreateSphere()
	{
		if (auto it = meshes.find(sphereID); it != meshes.end())
			return it->second;

		Ref<Mesh3D> mesh = CreateRef<Mesh3D>();
		auto& vertices = mesh->vertices;
		auto& indices = mesh->indices;

		// https://www.opengl.org.ru/docs/pg/0208.html

		static constexpr float x = 0.525731112119133606f;
		static constexpr float X = -x;
		static constexpr float z = 0.850650808352039932f;
		static constexpr float Z = -z;

		static constexpr auto verts = std::to_array({
			glm::vec3(X, 0, z), glm::vec3(x, 0, z), glm::vec3(X, 0, Z), glm::vec3(x, 0, Z),
			glm::vec3(0, z, x), glm::vec3(0, z, X), glm::vec3(0, Z, x), glm::vec3(0, Z, X),
			glm::vec3(z, x, 0), glm::vec3(Z, x, 0), glm::vec3(z, X, 0), glm::vec3(Z, X, 0)
		});

		static constexpr auto triangles = std::to_array({
			glm::vec3(0,  4,  1), glm::vec3(0, 9,  4), glm::vec3(9,  5, 4), glm::vec3( 4, 5, 8), glm::vec3(4, 8,  1),
			glm::vec3(8, 10,  1), glm::vec3(8, 3, 10), glm::vec3(5,  3, 8), glm::vec3( 5, 2, 3), glm::vec3(2, 7,  3),
			glm::vec3(7, 10,  3), glm::vec3(7, 6, 10), glm::vec3(7, 11, 6), glm::vec3(11, 0, 6), glm::vec3(0, 1,  6),
			glm::vec3(6,  1, 10), glm::vec3(9, 0, 11), glm::vec3(9, 11, 2), glm::vec3( 9, 2, 5), glm::vec3(7, 2, 11)
		});

		static constexpr uint32_t subdivisions = 3;
		std::srand(static_cast<uint32_t>(std::time(nullptr)));

		std::vector<Triangle> outTriangles;
		for (const glm::uvec3& triangle : triangles)
		{
			// Get vertices
			const glm::vec3& position0 = verts[triangle.x];
			const glm::vec3& position1 = verts[triangle.y];
			const glm::vec3& position2 = verts[triangle.z];

			Subdivide(outTriangles, position0, position1, position2, static_cast<uint32_t>(std::rand() % 4));
		}

		uint32_t vertexCount = 0;
		for (const Triangle& triangle : outTriangles)
		{
			// TODO: see if they face the correct way
			// Calculate normal
			glm::vec3 normal = glm::normalize(glm::cross(triangle.position1 - triangle.position0, triangle.position2 - triangle.position0));

			// TODO: texCoords
			vertices.emplace_back(triangle.position0, normal, glm::vec2(0.0f));
			vertices.emplace_back(triangle.position1, normal, glm::vec2(0.0f));
			vertices.emplace_back(triangle.position2, normal, glm::vec2(0.0f));
			indices.push_back(vertexCount + 0);
			indices.push_back(vertexCount + 1);
			indices.push_back(vertexCount + 2);
			vertexCount += 3;
		}

		meshes[sphereID] = mesh;
		return mesh;
	}

	Ref<Mesh3D> MeshFactory3D::CreatePlane()
	{
		if (auto it = meshes.find(planeID); it != meshes.end())
			return it->second;

		Ref<Mesh3D> mesh = CreateRef<Mesh3D>();
		auto& vertices = mesh->vertices;
		auto& indices = mesh->indices;
		//mesh->filepath = planeID;

		static constexpr glm::vec3 positionLeftBottom { -0.5f, 0.0f,  0.5f };
		static constexpr glm::vec3 positionRightBottom{  0.5f, 0.0f,  0.5f };
		static constexpr glm::vec3 positionLeftTop    { -0.5f, 0.0f, -0.5f };
		static constexpr glm::vec3 positionRightTop   {  0.5f, 0.0f, -0.5f };

		static constexpr glm::vec3 normal{ 0.0f, 1.0f, 0.0f };

		glm::vec2 texCoordLeftBottom { 0.0f, 1.0f };
		glm::vec2 texCoordRightBottom{ 1.0f, 1.0f };
		glm::vec2 texCoordLeftTop    { 0.0f, 0.0f };
		glm::vec2 texCoordRightTop   { 1.0f, 0.0f };
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			std::swap(texCoordLeftBottom, texCoordLeftTop);
			std::swap(texCoordRightBottom, texCoordRightTop);
		}

		vertices.emplace_back(positionLeftBottom,  normal, texCoordLeftBottom);
		vertices.emplace_back(positionRightBottom, normal, texCoordRightBottom);
		vertices.emplace_back(positionLeftTop,     normal, texCoordLeftTop);
		vertices.emplace_back(positionRightTop,    normal, texCoordRightTop);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(0);

		meshes[planeID] = mesh;
		return mesh;
	}

	Ref<Mesh3D> MeshFactory3D::CreateFromID(std::wstring_view id)
	{
		if (id.starts_with(cubeID)) return CreateCube();
		if (id.starts_with(sphereID)) return CreateSphere();
		if (id.starts_with(planeID)) return CreatePlane();
		return nullptr;
	}
}
