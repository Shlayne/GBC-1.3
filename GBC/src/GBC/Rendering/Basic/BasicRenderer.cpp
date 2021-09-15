#include "gbcpch.h"
#include "BasicRenderer.h"
#include "GBC/IO/FileIO.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/RendererCapabilities.h"
#include "GBC/Rendering/Shader.h"

namespace gbc
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 tintColor;
		glm::vec2 texCoord;
		uint32_t texIndex;
		float tilingFactor;
	};

	struct BasicRendererData
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		uint32_t localIndexCount = 0;
		uint32_t* localIndexBufferStart = nullptr;
		uint32_t* localIndexBufferCurrent = nullptr;
		uint32_t localVertexCount = 0;
		Vertex* localVertexBufferStart = nullptr;
		Vertex* localVertexBufferCurrent = nullptr;

		Ref<Texture2D>* textures = nullptr;
		uint32_t textureCount = 1;

		uint32_t maxVertices = 0;
		uint32_t maxIndices = 0;
		uint32_t maxTextures = 0;

#if GBC_ENABLE_STATS
		BasicRenderer::Statistics statistics;
#endif
	};
	static BasicRendererData data;

	void BasicRenderer::Init()
	{
		Renderer::EnableDepthTest();
		Renderer::EnableBlending();
		Renderer::EnableCullFace();

		// Setup texture slots
		data.maxTextures = static_cast<uint32_t>(RendererCapabilities::GetMaxTextureSlots());
		data.textures = new Ref<Texture2D>[data.maxTextures];

		// Setup local buffers
		data.maxVertices = 900000; // TODO: query drivers
		data.maxIndices  = 600000; // TODO: query drivers
		data.localVertexBufferStart = new Vertex[data.maxVertices];
		data.localVertexBufferCurrent = data.localVertexBufferStart;
		data.localIndexBufferStart = new uint32_t[data.maxIndices];
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Setup internal buffers
		data.vertexBuffer = VertexBuffer::Create(data.maxVertices * sizeof(Vertex), nullptr, BufferUsage::DynamicDraw);
		data.vertexBuffer->SetLayout({
			{ VertexBufferElementType::Float3, "position" },
			{ VertexBufferElementType::Float4, "tintColor" },
			{ VertexBufferElementType::Float2, "texCoord" },
			{ VertexBufferElementType::UInt,   "texIndex" },
			{ VertexBufferElementType::Float,  "tilingFactor" }
		});

		data.vertexArray = VertexArray::Create();
		data.vertexArray->AddVertexBuffer(data.vertexBuffer);

		data.indexBuffer = IndexBuffer::Create(data.maxIndices, nullptr, BufferUsage::DynamicDraw, IndexBufferElementType::UInt32);

		// Setup shader
		data.shader = Shader::Create("Resources/Shaders/BasicShader.glsl");
		data.shader->Bind();
		int32_t* samplers = new int32_t[data.maxTextures];
		for (uint32_t i = 0; i < data.maxTextures; i++)
			samplers[i] = static_cast<int32_t>(i);
		data.shader->SetInts("textures", samplers, data.maxTextures);
		delete[] samplers;

		// Setup white texture
		auto whiteTexture = LocalTexture2D::Create(1, 1, 4);
		*(uint32_t*)whiteTexture->GetData() = 0xFFFFFFFF;
		data.textures[0] = Texture2D::Create(whiteTexture);
	}

	void BasicRenderer::Shutdown()
	{
		delete[] data.localVertexBufferStart;
		delete[] data.localIndexBufferStart;
		delete[] data.textures;
	}

	void BasicRenderer::BeginScene(const Camera& camera, const glm::mat4& view)
	{
		// Set shader uniforms
		data.shader->Bind();
		data.shader->SetMat4("viewProjection", camera.GetProjection() * view);
	}

	void BasicRenderer::BeginScene(const EditorCamera& camera)
	{
		// Set shader uniforms
		data.shader->Bind();
		data.shader->SetMat4("viewProjection", camera.GetViewProjection());
	}

	void BasicRenderer::EndScene()
	{
		uint32_t vertexBufferSize = static_cast<uint32_t>((data.localVertexCount * sizeof(Vertex)));
		uint32_t indexBufferSize = static_cast<uint32_t>((data.localIndexCount * sizeof(uint32_t)));

		if (vertexBufferSize != 0 && indexBufferSize != 0)
		{
			// Copy local buffers to internal buffers
			data.vertexBuffer->SetData(data.localVertexBufferStart, vertexBufferSize);
			data.indexBuffer->SetData(data.localIndexBufferStart, indexBufferSize);

			// Bind renderables
			for (uint32_t i = 0; i < data.textureCount; i++)
				data.textures[i]->Bind(i);

			// Actually render
			Renderer::DrawIndexed(data.vertexArray, data.indexBuffer, 0, data.localIndexCount);

#if GBC_ENABLE_STATS
			data.statistics.drawCalls++;
#endif
		}

		Reset();
	}

	void BasicRenderer::EnsureBatch(uint32_t vertexCount, uint32_t indexCount, uint32_t texIndex)
	{
		if (data.localVertexCount + vertexCount > data.maxVertices ||
			data.localIndexCount + indexCount > data.maxIndices ||
			texIndex >= data.maxTextures)
			EndScene();
	}

	void BasicRenderer::Reset()
	{
		// Reset local buffers
		data.localVertexCount = 0;
		data.localVertexBufferCurrent = data.localVertexBufferStart;

		data.localIndexCount = 0;
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Remove the references once the textures has been rendered
		for (uint32_t i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	uint32_t BasicRenderer::GetTexIndex(const Ref<Texture2D>& texture)
	{
		if (!texture || !texture->GetTexture() || !*texture->GetTexture())
			return 0;

		for (uint32_t i = 1; i < data.textureCount; i++)
			if (*data.textures[i]->GetTexture() == *texture->GetTexture())
				return i;

		return data.textureCount;
	}

	void BasicRenderer::Submit(const Ref<BasicMesh>& mesh, const glm::mat4& transform, const RenderableComponent& renderableComponent)
	{
		// Handle renderable
		uint32_t vertexCount = static_cast<uint32_t>(mesh->vertices.size());
		uint32_t indexCount = static_cast<uint32_t>(mesh->indices.size());
		auto& texture = renderableComponent.texture;
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
			data.localVertexBufferCurrent->position = transform * glm::vec4(mesh->vertices[i].position, 1.0f);
			data.localVertexBufferCurrent->tintColor = mesh->vertices[i].tintColor * renderableComponent.color;
			data.localVertexBufferCurrent->texCoord = mesh->vertices[i].texCoord;
			data.localVertexBufferCurrent->texIndex = texIndex;
			data.localVertexBufferCurrent->tilingFactor = renderableComponent.tilingFactor;
		}

		// Handle indices
		for (uint32_t i = 0; i < indexCount; i++, data.localIndexBufferCurrent++)
			*data.localIndexBufferCurrent = data.localVertexCount + mesh->indices[i];

		// Update local counts
		data.localVertexCount += vertexCount;
		data.localIndexCount += indexCount;

#if GBC_ENABLE_STATS
		data.statistics.indexCount += indexCount;
		data.statistics.vertexCount += vertexCount;
#endif
	}

#if GBC_ENABLE_STATS
	const BasicRenderer::Statistics& BasicRenderer::GetStatistics()
	{
		return data.statistics;
	}

	void BasicRenderer::ResetStatistics()
	{
		data.statistics.drawCalls = 0;
		data.statistics.indexCount = 0;
		data.statistics.vertexCount = 0;
		data.statistics.textureCount = 0;
	}
#endif
}
