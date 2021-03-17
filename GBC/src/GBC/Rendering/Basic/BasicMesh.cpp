#include "gbcpch.h"
#include "BasicMesh.h"

namespace gbc
{
	BasicMesh::BasicMesh(unsigned int vertexCount, unsigned int indexCount)
		: vertexCount(vertexCount), indexCount(indexCount)
	{
		vertices = new BasicVertex[vertexCount];
		indices = new unsigned int[indexCount];
	}

	BasicMesh::BasicMesh(const BasicMesh& mesh)
		: vertexCount(mesh.vertexCount), indexCount(mesh.indexCount)
	{
		vertices = new BasicVertex[vertexCount];
		indices = new unsigned int[indexCount];
		memcpy_s(vertices, vertexCount, mesh.vertices, vertexCount);
		memcpy_s(indices, indexCount, mesh.indices, indexCount);
	}

	BasicMesh::BasicMesh(BasicMesh&& mesh) noexcept
		: vertexCount(mesh.vertexCount), indexCount(mesh.indexCount), vertices(mesh.vertices), indices(mesh.indices)
	{
		mesh.vertexCount = 0;
		mesh.indexCount = 0;
		mesh.vertices = nullptr;
		mesh.indices = nullptr;
	}

	BasicMesh::~BasicMesh()
	{
		delete[] vertices;
		delete[] indices;
	}

	BasicMesh& BasicMesh::operator=(BasicMesh&& mesh) noexcept
	{
		vertexCount = mesh.vertexCount;
		indexCount = mesh.indexCount;
		vertices = mesh.vertices;
		indices = mesh.indices;
		mesh.vertexCount = 0;
		mesh.indexCount = 0;
		mesh.vertices = nullptr;
		mesh.indices = nullptr;
		return *this;
	}

	BasicMesh::BasicMesh(OBJModel&& mesh) noexcept
	{
		vertexCount = (unsigned int)mesh.indices.size();
		indexCount = vertexCount;
		vertices = new BasicVertex[vertexCount];
		indices = new unsigned int[indexCount];
		filepath = mesh.filepath;

		for (unsigned int i = 0; i < (unsigned int)mesh.indices.size(); i++)
		{
			vertices[i].position = mesh.positions[mesh.indices[i].x];
			vertices[i].texCoord = mesh.texCoords[mesh.indices[i].y];
			indices[i] = i;
		}

		mesh.Clear();
	}


	void BasicMesh::Create(unsigned int vertexCount, unsigned int indexCount)
	{
		if (this->vertexCount == 0)
		{
			delete[] vertices;
			delete[] indices;
		}

		this->vertexCount = vertexCount;
		this->indexCount = indexCount;
		vertices = new BasicVertex[vertexCount];
		indices = new unsigned int[indexCount];
	}
}
