#include "gbcpch.h"
#include "BasicRenderer.h"
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
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		struct CameraBuffer
		{
			glm::mat4 viewProjection{ 1.0f };
		};
		CameraBuffer cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;

#if GBC_ENABLE_STATS
		BasicRenderer::Statistics statistics;
#endif
	};
	static BasicRendererData data;

	void BasicRenderer::Init()
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
			{ VertexBufferElementType::Float3, "position" },
			{ VertexBufferElementType::Float4, "tintColor" },
			{ VertexBufferElementType::Float2, "texCoord" },
			{ VertexBufferElementType::UInt,   "texIndex" },
			{ VertexBufferElementType::Float,  "tilingFactor" }
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

		data.cameraUniformBuffer = UniformBuffer::Create(sizeof(BasicRendererData::CameraBuffer), 0, nullptr, BufferUsage::DynamicDraw);

		data.shader = Shader::Create("Resources/Shaders/BasicShader.glsl");

		// Setup white texture
		auto whiteTexture = LocalTexture2D::Create(1, 1, 4);
		*(uint32_t*)whiteTexture->GetData() = 0xFFFFFFFF;
		data.textures[0] = Texture2D::Create(whiteTexture);
	}

	void BasicRenderer::Shutdown()
	{
		delete[] data.localVertexBufferStart;

		// Make sure to free GPU memory
		delete[] data.textures;
		data.shader.reset();
		data.vertexArray.reset();
		data.vertexBuffer.reset();
		data.indexBuffer.reset();
		data.cameraUniformBuffer.reset();
	}

	void BasicRenderer::BeginScene(const Camera& camera, const glm::mat4& view)
	{
		// Set shader uniforms
		data.cameraBuffer.viewProjection = camera.GetProjection() * view;
		data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(BasicRendererData::CameraBuffer));
	}

	void BasicRenderer::BeginScene(const EditorCamera& camera)
	{
		// Set shader uniforms
		data.cameraBuffer.viewProjection = camera.GetViewProjection();
		data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(BasicRendererData::CameraBuffer));
	}

	void BasicRenderer::EndScene()
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

	void BasicRenderer::Reset()
	{
		data.quadCount = 0;
		data.localVertexBufferCurrent = data.localVertexBufferStart;

		// Remove the references once the textures has been rendered
		for (uint32_t i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	void BasicRenderer::DrawQuad(const glm::vec3& translation, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void BasicRenderer::DrawQuad(const glm::vec3& translation, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void BasicRenderer::DrawQuad(const glm::vec3& translation, const glm::vec2& scale, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		DrawQuad(transform, color, texture, tilingFactor);
	}

	void BasicRenderer::DrawQuad(const glm::vec3& translation, float rotation, const glm::vec2& scale, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f))
							* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		DrawQuad(transform, color, texture, tilingFactor);
	}

	uint32_t BasicRenderer::GetTexIndex(const Ref<Texture2D>& texture)
	{
		// If texture is null or it has a null local texture, then use white texture
		if (!texture || (!texture->IsFramebufferTexture() && (!texture->GetTexture() || !*texture->GetTexture())))
			return 0;

		for (uint32_t i = 1; i < data.textureCount; i++)
			if (*data.textures[i]->GetTexture() == *texture->GetTexture())
				return i;

		return data.textureCount;
	}

	void BasicRenderer::EnsureBatch(uint32_t texIndex)
	{
		if (data.quadCount + 1 > data.maxQuads && texIndex >= data.maxTextures)
			EndScene();
	}

	void BasicRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor)
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
		for (uint32_t i = 0; i < 4; i++, data.localVertexBufferCurrent++)
		{
			data.localVertexBufferCurrent->position = transform * data.quadVertexPositions[i];
			data.localVertexBufferCurrent->tintColor = color;
			data.localVertexBufferCurrent->texCoord = data.quadVertexTexCoords[i];
			data.localVertexBufferCurrent->texIndex = texIndex;
			data.localVertexBufferCurrent->tilingFactor = tilingFactor;
		}

		// Update counts
		data.quadCount++;
#if GBC_ENABLE_STATS
		data.statistics.quadCount++;
#endif
	}

#if GBC_ENABLE_STATS
	const BasicRenderer::Statistics& BasicRenderer::GetStatistics()
	{
		return data.statistics;
	}

	void BasicRenderer::ResetStatistics()
	{
		data.statistics.drawCallCount = 0;
		data.statistics.quadCount = 0;
		data.statistics.textureCount = 0;
	}
#endif
}
