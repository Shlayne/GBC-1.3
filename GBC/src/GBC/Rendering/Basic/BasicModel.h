#pragma once

#include "BasicVertex.h"

namespace gbc
{
	struct BasicModel
	{
		BasicModel() = default;
		~BasicModel();
		BasicModel(unsigned int vertexCount, unsigned int indexCount);
		BasicModel(const BasicModel& model);
		BasicModel(BasicModel&& model) noexcept;
		BasicModel& operator=(BasicModel&& model) noexcept;

		void Create(unsigned int vertexCount, unsigned int indexCount);

		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
		BasicVertex* vertices = nullptr;
		unsigned int* indices = nullptr;
	};
}
