#pragma once

#include "BasicVertex.h"
#include "GBC/IO/OBJLoader.h"

namespace gbc
{
	// TODO: refactor this
	struct BasicMesh
	{
		BasicMesh() = default;
		~BasicMesh();
		BasicMesh(unsigned int vertexCount, unsigned int indexCount);
		BasicMesh(const BasicMesh& mesh);
		BasicMesh(BasicMesh&& mesh) noexcept;
		BasicMesh& operator=(BasicMesh&& mesh) noexcept;
		BasicMesh(OBJModel&& mesh) noexcept;

		void Create(unsigned int vertexCount, unsigned int indexCount);

		unsigned int vertexCount = 0;
		unsigned int indexCount = 0;
		BasicVertex* vertices = nullptr;
		unsigned int* indices = nullptr;
		std::string filepath;
	};
}
