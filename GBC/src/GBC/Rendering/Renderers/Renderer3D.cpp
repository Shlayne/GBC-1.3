#include "gbcpch.h"
#include "Renderer3D.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/RendererCapabilities.h"
#include "GBC/Rendering/Shader.h"

namespace gbc
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
		uint32_t texIndex;
	};

	struct Renderer3DData
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		uint32_t localVertexCount = 0;
		QuadVertex* localVertexBufferStart = nullptr;
		QuadVertex* localVertexBufferCurrent = nullptr;
		uint32_t localIndexCount = 0;
		uint32_t* localIndexBufferStart = nullptr;
		uint32_t* localIndexBufferCurrent = nullptr;

		Ref<Texture2D>* textures = nullptr;
		uint32_t textureCount = 1;
		uint32_t maxTextures = 0;

		uint32_t maxVertices = 0;
		uint32_t maxIndices = 0;

#if GBC_ENABLE_STATS
		Renderer3D::Statistics statistics;
#endif
	};
	static Renderer3DData data;

	void Renderer3D::Init()
	{
		Renderer::EnableDepthTest();
		Renderer::EnableBlending();
		Renderer::EnableCullFace();

		// Setup texture slots
		data.maxTextures = static_cast<uint32_t>(RendererCapabilities::GetMaxTextureSlots());
		data.textures = new Ref<Texture2D>[data.maxTextures];

		// Setup local buffers
		data.maxVertices = 9000000; // TODO: query drivers
		data.maxIndices  = 6000000; // TODO: query drivers
		data.localVertexBufferStart = new QuadVertex[data.maxVertices];
		data.localVertexBufferCurrent = data.localVertexBufferStart;
		data.localIndexBufferStart = new uint32_t[data.maxIndices];
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Setup buffers
		data.vertexBuffer = VertexBuffer::Create(data.maxVertices * sizeof(QuadVertex), nullptr, BufferUsage::DynamicDraw);
		data.vertexBuffer->SetLayout({
			{ VertexBufferElementType::Float3, "position"  },
			{ VertexBufferElementType::Float3, "normal"    },
			{ VertexBufferElementType::Float2, "texCoord"  },
			{ VertexBufferElementType::UInt,   "texIndex"  }
		});

		data.vertexArray = VertexArray::Create();
		data.vertexArray->AddVertexBuffer(data.vertexBuffer);

		data.indexBuffer = IndexBuffer::Create(data.maxIndices, nullptr, BufferUsage::DynamicDraw, IndexBufferElementType::UInt32);

		data.shader = Shader::Create("Resources/Shaders/Renderer3D.glsl");

		// Setup white texture
		auto whiteTexture = LocalTexture2D::Create(1, 1, 4);
		*(uint32_t*)whiteTexture->GetData() = 0xFFFFFFFF;
		data.textures[0] = Texture2D::Create(whiteTexture);
	}

	void Renderer3D::Shutdown()
	{
		delete[] data.localVertexBufferStart;
		delete[] data.localIndexBufferStart;

		// Make sure to free GPU memory
		delete[] data.textures;
		data.shader.reset();
		data.vertexArray.reset();
		data.vertexBuffer.reset();
		data.indexBuffer.reset();
	}

	void Renderer3D::BeginScene(const glm::mat4& viewProjection)
	{
		Renderer::CameraBuffer cameraBuffer;
		cameraBuffer.viewProjection = viewProjection;
		Renderer::UploadCameraBuffer(cameraBuffer);
	}

	void Renderer3D::EndScene()
	{
		uint32_t vertexBufferSize = static_cast<uint32_t>(data.localVertexCount * sizeof(QuadVertex));
		uint32_t indexBufferSize = static_cast<uint32_t>(data.localIndexCount * sizeof(uint32_t));
		if (vertexBufferSize != 0)
		{
			// Copy vertices and indices to GPU
			data.vertexBuffer->SetData(data.localVertexBufferStart, vertexBufferSize);
			data.indexBuffer->SetData(data.localIndexBufferStart, indexBufferSize);

			// Bind textures
			for (uint32_t i = 0; i < data.textureCount; i++)
				data.textures[i]->Bind(i);

			// Actually render
			data.shader->Bind();
			Renderer::DrawIndexed(data.vertexArray, data.indexBuffer, 0, data.localIndexCount);

#if GBC_ENABLE_STATS
			data.statistics.drawCallCount++;
#endif
		}

		Reset();
	}

	void Renderer3D::Reset()
	{
		// Reset local buffers
		data.localVertexCount = 0;
		data.localVertexBufferCurrent = data.localVertexBufferStart;

		data.localIndexCount = 0;
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Remove the texture references once they've been used
		for (uint32_t i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	uint32_t Renderer3D::GetTexIndex(const Ref<Texture2D>& texture)
	{
		if (!texture || !texture->GetTexture() || !*texture->GetTexture())
			return 0;

		for (uint32_t i = 1; i < data.textureCount; i++)
			if (*data.textures[i]->GetTexture() == *texture->GetTexture())
				return i;

		return data.textureCount;
	}

	void Renderer3D::EnsureBatch(uint32_t vertexCount, uint32_t indexCount, uint32_t texIndex)
	{
		if (data.localVertexCount + vertexCount > data.maxVertices ||
			data.localIndexCount + indexCount > data.maxIndices ||
			texIndex >= data.maxTextures)
			EndScene();
	}

	void Renderer3D::Submit(const Ref<Model3D>& model, const glm::mat4& transform)
	{
		// TODO: material textures and local mesh transforms
		for (const auto& mesh : model->GetMeshes())
			Submit(mesh, transform, nullptr);
	}

	void Renderer3D::Submit(const Mesh3D& mesh, const glm::mat4& transform, const Ref<Texture2D>& texture)
	{
		// Handle texture
		uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
		uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());
		uint32_t texIndex = GetTexIndex(texture);

		EnsureBatch(vertexCount, indexCount, texIndex);
		if (texIndex >= data.textureCount)
		{
			data.textures[texIndex = data.textureCount++] = texture;
#if GBC_ENABLE_STATS
			data.statistics.textureCount++;
#endif
		}

		// Handle vertices
		for (uint32_t i = 0; i < vertexCount; i++, data.localVertexBufferCurrent++)
		{
			data.localVertexBufferCurrent->position = transform * glm::vec4(mesh.vertices[i].position, 1.0f);
			data.localVertexBufferCurrent->normal = mesh.vertices[i].normal;
			data.localVertexBufferCurrent->texCoord = mesh.vertices[i].texCoord;
			data.localVertexBufferCurrent->texIndex = texIndex;
		}

		// Handle indices
		for (uint32_t i = 0; i < indexCount; i++, data.localIndexBufferCurrent++)
			*data.localIndexBufferCurrent = data.localVertexCount + mesh.indices[i];

		// Update local counts
		data.localVertexCount += vertexCount;
		data.localIndexCount += indexCount;

#if GBC_ENABLE_STATS
		data.statistics.indexCount += indexCount;
		data.statistics.vertexCount += vertexCount;
		data.statistics.meshCount++;
#endif
	}

#if GBC_ENABLE_STATS
	const Renderer3D::Statistics& Renderer3D::GetStatistics()
	{
		return data.statistics;
	}

	void Renderer3D::ResetStatistics()
	{
		std::memset(&data.statistics, 0, sizeof(data.statistics));
	}
#endif
}
