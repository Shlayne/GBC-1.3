#include "gbcpch.h"
#include "BasicMesh.h"

namespace gbc
{
	BasicMesh::BasicMesh(uint32_t vertexCount, uint32_t indexCount)
		: vertices(vertexCount), indices(indexCount) {}

	BasicMesh::BasicMesh(const BasicMesh& mesh)
		: vertices(mesh.vertices.size()), indices(mesh.indices.size())
	{
		memcpy_s(&vertices[0], vertices.size(), &mesh.vertices[0], mesh.vertices.size());
		memcpy_s(&indices[0], indices.size(), &mesh.indices[0], mesh.indices.size());
	}

	BasicMesh::BasicMesh(BasicMesh&& mesh) noexcept
		:vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)) {}

	BasicMesh& BasicMesh::operator=(BasicMesh&& mesh) noexcept
	{
		if (this != &mesh)
		{
			vertices = std::move(mesh.vertices);
			indices = std::move(mesh.indices);
		}
		return *this;
	}

	BasicMesh::BasicMesh(const OBJModel& model)
		: vertices(model.indices.size()), indices(model.indices.size())
	{
		if (model.texCoords.empty())
		{
			for (size_t i = 0; i < model.indices.size(); i++)
			{
				vertices[i].position = model.positions[model.indices[i].x];
				indices[i] = static_cast<uint32_t>(i);
			}
		}
		else
		{
			for (size_t i = 0; i < model.indices.size(); i++)
			{
				vertices[i].position = model.positions[model.indices[i].x];
				vertices[i].texCoord = model.texCoords[model.indices[i].y];
				indices[i] = static_cast<uint32_t>(i);
			}
		}
	}

	BasicMesh::BasicMesh(OBJModel&& model) noexcept
		: vertices(model.indices.size()), indices(model.indices.size())
	{
		if (model.texCoords.empty())
		{
			for (size_t i = 0; i < model.indices.size(); i++)
			{
				vertices[i].position = model.positions[model.indices[i].x];
				indices[i] = static_cast<uint32_t>(i);
			}
		}
		else
		{
			for (size_t i = 0; i < model.indices.size(); i++)
			{
				vertices[i].position = model.positions[model.indices[i].x];
				vertices[i].texCoord = model.texCoords[model.indices[i].y];
				indices[i] = static_cast<uint32_t>(i);
			}
		}
	}

	void BasicMesh::Create(uint32_t vertexCount, uint32_t indexCount)
	{
		vertices.clear();
		indices.clear();
		vertices.reserve(vertexCount);
		indices.reserve(indexCount);
	}
}
