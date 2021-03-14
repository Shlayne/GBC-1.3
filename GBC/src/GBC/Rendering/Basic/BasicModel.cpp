#include "gbcpch.h"
#include "BasicModel.h"

namespace gbc
{
	BasicModel::BasicModel(unsigned int vertexCount, unsigned int indexCount)
		: vertexCount(vertexCount), indexCount(indexCount)
	{
		vertices = new BasicVertex[vertexCount];
		indices = new unsigned int[indexCount];
	}

	BasicModel::BasicModel(const BasicModel& model)
		: vertexCount(model.vertexCount), indexCount(model.indexCount)
	{
		vertices = new BasicVertex[vertexCount];
		indices = new unsigned int[indexCount];
		memcpy_s(vertices, vertexCount, model.vertices, vertexCount);
		memcpy_s(indices, indexCount, model.indices, indexCount);
	}

	BasicModel::BasicModel(BasicModel&& model) noexcept
		: vertexCount(model.vertexCount), indexCount(model.indexCount), vertices(model.vertices), indices(model.indices)
	{
		model.vertexCount = 0;
		model.indexCount = 0;
		model.vertices = nullptr;
		model.indices = nullptr;
	}

	BasicModel::~BasicModel()
	{
		delete[] vertices;
		delete[] indices;
	}

	BasicModel& BasicModel::operator=(BasicModel&& model) noexcept
	{
		vertexCount = model.vertexCount;
		indexCount = model.indexCount;
		vertices = model.vertices;
		indices = model.indices;
		model.vertexCount = 0;
		model.indexCount = 0;
		model.vertices = nullptr;
		model.indices = nullptr;
		return *this;
	}

	void BasicModel::Create(unsigned int vertexCount, unsigned int indexCount)
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
