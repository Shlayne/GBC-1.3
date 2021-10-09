#include "gbcpch.h"
#include "Renderer2D.h"
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
		glm::vec2 tilingFactor;
	};

	struct Renderer2DData
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		Vertex* localVertexBufferStart = nullptr;
		Vertex* localVertexBufferCurrent = nullptr;

		Ref<Texture2D>* textures = nullptr;
		uint32_t textureCount = 1;
		uint32_t maxTextures = 0;

		static constexpr uint32_t maxQuads = 65536;
		static constexpr uint32_t maxVertices = maxQuads * 4;
		static constexpr uint32_t maxIndices = maxQuads * 6;

		uint32_t quadCount = 0;
		static constexpr glm::vec4 quadVertexPositions[4]
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{ +0.5f, -0.5f, 0.0f, 1.0f },
			{ +0.5f, +0.5f, 0.0f, 1.0f },
			{ -0.5f, +0.5f, 0.0f, 1.0f }
		};
		static constexpr glm::vec2 quadVertexTexCoords[4]
		{
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 0.0f }
		};

#if GBC_ENABLE_STATS
		Renderer2D::Statistics statistics;
#endif
	};
	static Renderer2DData data;

	void Renderer2D::Init()
	{
		Renderer::DisableDepthTest();
		Renderer::EnableCullFace();
		Renderer::EnableBlending();

		// Setup texture slots
		data.maxTextures = static_cast<uint32_t>(RendererCapabilities::GetMaxTextureSlots());
		data.textures = new Ref<Texture2D>[data.maxTextures];

		// Setup vertex buffer
		data.localVertexBufferStart = new Vertex[data.maxVertices];
		data.localVertexBufferCurrent = data.localVertexBufferStart;
		data.vertexBuffer = VertexBuffer::Create(data.maxVertices * sizeof(Vertex), nullptr, BufferUsage::DynamicDraw);
		data.vertexBuffer->SetLayout({
			{ VertexBufferElementType::Float3, "position"     },
			{ VertexBufferElementType::Float4, "tintColor"    },
			{ VertexBufferElementType::Float2, "texCoord"     },
			{ VertexBufferElementType::UInt,   "texIndex"     },
			{ VertexBufferElementType::Float2, "tilingFactor" }
		});

		data.vertexArray = VertexArray::Create();
		data.vertexArray->AddVertexBuffer(data.vertexBuffer);

		// Setup index buffer
		uint32_t* indices = new uint32_t[data.maxIndices];
		for (uint32_t index = 0, offset = 0; index < data.maxIndices; index += 6, offset += 4)
		{
			indices[index + 0] = offset + 0;
			indices[index + 1] = offset + 1;
			indices[index + 2] = offset + 2;
			indices[index + 3] = offset + 2;
			indices[index + 4] = offset + 3;
			indices[index + 5] = offset + 0;
		}
		data.indexBuffer = IndexBuffer::Create(data.maxIndices, indices);
		delete[] indices;

		data.shader = Shader::Create("Resources/Shaders/Renderer2D.glsl");

		// Setup white texture
		auto whiteTexture = LocalTexture2D::Create(1, 1, 4);
		*(uint32_t*)whiteTexture->GetData() = 0xFFFFFFFF;
		data.textures[0] = Texture2D::Create(whiteTexture);
	}

	void Renderer2D::Shutdown()
	{
		delete[] data.localVertexBufferStart;

		// Make sure to free GPU memory
		delete[] data.textures;
		data.shader.reset();
		data.vertexArray.reset();
		data.vertexBuffer.reset();
		data.indexBuffer.reset();
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjection)
	{
		Renderer::CameraBuffer cameraBuffer;
		cameraBuffer.viewProjection = viewProjection;
		Renderer::UploadCameraBuffer(cameraBuffer);
	}

	void Renderer2D::EndScene()
	{
		uint32_t vertexBufferSize = static_cast<uint32_t>((data.quadCount * (4 * sizeof(Vertex))));
		if (vertexBufferSize != 0)
		{
			// Copy vertices to GPU
			data.vertexBuffer->SetData(data.localVertexBufferStart, vertexBufferSize);

			// Bind textures
			for (uint32_t i = 0; i < data.textureCount; i++)
				data.textures[i]->Bind(i);

			// Actually render
			data.shader->Bind();
			Renderer::DrawIndexed(data.vertexArray, data.indexBuffer, 0, data.quadCount * 6);

#if GBC_ENABLE_STATS
			data.statistics.drawCallCount++;
#endif
		}

		Reset();
	}

	void Renderer2D::Reset()
	{
		data.quadCount = 0;
		data.localVertexBufferCurrent = data.localVertexBufferStart;

		// Remove the references once the textures has been rendered
		for (uint32_t i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	uint32_t Renderer2D::GetTexIndex(const Ref<Texture2D>& texture)
	{
		// If texture is null or it has a null local texture, then use white texture
		if (!texture || (!texture->IsFramebufferTexture() && (!texture->GetTexture() || !*texture->GetTexture())))
			return 0;

		for (uint32_t i = 1; i < data.textureCount; i++)
			if (*data.textures[i]->GetTexture() == *texture->GetTexture())
				return i;

		return data.textureCount;
	}

	void Renderer2D::EnsureBatch(uint32_t texIndex)
	{
		if (data.quadCount >= data.maxQuads || texIndex >= data.maxTextures)
			EndScene();
	}

	void Renderer2D::DrawQuad(const glm::vec3& translation, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& translation, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& translation, const glm::vec2& scale, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& translation, float rotation, const glm::vec2& scale, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		// Handle texture
		uint32_t texIndex = GetTexIndex(texture);
		EnsureBatch(texIndex);
		if (texIndex >= data.textureCount)
		{
			texIndex = data.textureCount;
			data.textures[texIndex] = texture;
			data.textureCount++;
#if GBC_ENABLE_STATS
			data.statistics.textureCount++;
#endif
		}

		// Handle vertices
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			for (uint32_t i = 0; i < 4; i++, data.localVertexBufferCurrent++)
			{
				data.localVertexBufferCurrent->position = transform * data.quadVertexPositions[i];
				data.localVertexBufferCurrent->tintColor = color;
				glm::vec2 texCoord = data.quadVertexTexCoords[i];
				texCoord.y = 1.0f - texCoord.y;
				data.localVertexBufferCurrent->texCoord = texCoord;
				data.localVertexBufferCurrent->texIndex = texIndex;
				data.localVertexBufferCurrent->tilingFactor = tilingFactor;
			}
		}
		else
		{
			for (uint32_t i = 0; i < 4; i++, data.localVertexBufferCurrent++)
			{
				data.localVertexBufferCurrent->position = transform * data.quadVertexPositions[i];
				data.localVertexBufferCurrent->tintColor = color;
				data.localVertexBufferCurrent->texCoord = data.quadVertexTexCoords[i];
				data.localVertexBufferCurrent->texIndex = texIndex;
				data.localVertexBufferCurrent->tilingFactor = tilingFactor;
			}
		}

		// Update counts
		data.quadCount++;
#if GBC_ENABLE_STATS
		data.statistics.quadCount++;
#endif
	}

#if GBC_ENABLE_STATS
	const Renderer2D::Statistics& Renderer2D::GetStatistics()
	{
		return data.statistics;
	}

	void Renderer2D::ResetStatistics()
	{
		std::memset(&data.statistics, 0, sizeof(data.statistics));
	}
#endif
}
