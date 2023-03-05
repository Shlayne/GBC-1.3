#include "gbcpch.h"
#include "Renderer2D.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/RendererCapabilities.h"
#include "GBC/Rendering/Shader.h"

namespace gbc
{
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 tintColor;
		glm::vec2 texCoord;
		uint32_t texIndex;
		glm::vec2 tilingFactor;
	};

	struct CircleVertex
	{
		glm::vec3 position;
		glm::vec2 localPosition;
		float thickness;
		glm::vec4 color;
	};

	struct Renderer2DData
	{
		// Quads
		Ref<Shader> quadShader;
		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<IndexBuffer> quadIndexBuffer;

		QuadVertex* localQuadVertexBufferStart = nullptr;
		QuadVertex* localQuadVertexBufferCurrent = nullptr;

		static constexpr uint32_t maxQuads = 65536;
		static constexpr uint32_t maxQuadVertices = maxQuads * 4;
		static constexpr uint32_t maxQuadIndices = maxQuads * 6;

		uint32_t quadCount = 0;
		static constexpr uint32_t quadVertexCount = 4;
		static constexpr glm::vec4 quadVertexPositions[quadVertexCount]
		{
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f, -0.5f, 0.0f, 1.0f },
			{  0.5f,  0.5f, 0.0f, 1.0f },
			{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		glm::vec2 quadVertexTexCoords[4]; // Don't initialize here

		// Textures
		Ref<Texture2D>* textures = nullptr;
		uint32_t textureCount = 1;
		uint32_t maxTextures = 0;

		// Circles
		Ref<Shader> circleShader;
		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;

		CircleVertex* localCircleVertexBufferStart = nullptr;
		CircleVertex* localCircleVertexBufferCurrent = nullptr;

		uint32_t circleCount = 0;

#if GBC_ENABLE_STATS
		Renderer2D::Statistics statistics;
#endif
	};
	static Renderer2DData data;

	void Renderer2D::Init()
	{
		GBC_PROFILE_FUNCTION();

		Renderer::DisableDepthTest();
		Renderer::EnableCullFace();
		Renderer::EnableBlending();

		// Setup vertex buffers
		data.localQuadVertexBufferStart = new QuadVertex[data.maxQuadVertices];
		data.localQuadVertexBufferCurrent = data.localQuadVertexBufferStart;
		data.quadVertexBuffer = VertexBuffer::Create(data.maxQuadVertices * sizeof(QuadVertex), nullptr, BufferUsage::DynamicDraw);
		data.quadVertexBuffer->SetLayout({
			{ VertexBufferElementType::Float3, "position"     },
			{ VertexBufferElementType::Float4, "tintColor"    },
			{ VertexBufferElementType::Float2, "texCoord"     },
			{ VertexBufferElementType::UInt,   "texIndex"     },
			{ VertexBufferElementType::Float2, "tilingFactor" }
		});

		data.localCircleVertexBufferStart = new CircleVertex[data.maxQuadVertices];
		data.localCircleVertexBufferCurrent = data.localCircleVertexBufferStart;
		data.circleVertexBuffer = VertexBuffer::Create(data.maxQuadVertices * sizeof(CircleVertex), nullptr, BufferUsage::DynamicDraw);
		data.circleVertexBuffer->SetLayout({
			{ VertexBufferElementType::Float3, "position"      },
			{ VertexBufferElementType::Float2, "localPosition" },
			{ VertexBufferElementType::Float,  "thickness"     },
			{ VertexBufferElementType::Float4, "color"         }
		});

		// Setup vertex arrays
		data.quadVertexArray = VertexArray::Create();
		data.quadVertexArray->AddVertexBuffer(data.quadVertexBuffer);
		data.circleVertexArray = VertexArray::Create();
		data.circleVertexArray->AddVertexBuffer(data.circleVertexBuffer);

		// Setup quad index buffer
		uint32_t* indices = new uint32_t[data.maxQuadIndices];
		for (uint32_t index = 0, offset = 0; index < data.maxQuadIndices; index += 6, offset += 4)
		{
			indices[index + 0] = offset + 0;
			indices[index + 1] = offset + 1;
			indices[index + 2] = offset + 2;
			indices[index + 3] = offset + 2;
			indices[index + 4] = offset + 3;
			indices[index + 5] = offset + 0;
		}
		data.quadIndexBuffer = IndexBuffer::Create(data.maxQuadIndices, indices, BufferUsage::StaticDraw, IndexBufferElementType::UInt32);
		delete[] indices;

		// Setup shaders
		data.quadShader = Shader::Create("Resources/Shaders/Renderer2DQuad.glsl");
		data.circleShader = Shader::Create("Resources/Shaders/Renderer2DCircle.glsl");

		// Setup texture slots
		data.maxTextures = static_cast<uint32_t>(RendererCapabilities::GetMaxTextureSlots());
		data.textures = new Ref<Texture2D>[data.maxTextures];

		// Setup white texture
		auto whiteTexture = LocalTexture2D::Create(1, 1, 4);
		*(uint32_t*)whiteTexture->GetData() = 0xFFFFFFFF;
		data.textures[0] = Texture2D::Create(whiteTexture);

		// Setup vertex positions
		if (RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
		{
			data.quadVertexTexCoords[0] = { 0.0f, 0.0f };
			data.quadVertexTexCoords[1] = { 1.0f, 0.0f };
			data.quadVertexTexCoords[2] = { 1.0f, 1.0f };
			data.quadVertexTexCoords[3] = { 0.0f, 1.0f };
		}
		else
		{
			data.quadVertexTexCoords[0] = { 0.0f, 1.0f };
			data.quadVertexTexCoords[1] = { 1.0f, 1.0f };
			data.quadVertexTexCoords[2] = { 1.0f, 0.0f };
			data.quadVertexTexCoords[3] = { 0.0f, 0.0f };
		}
	}

	void Renderer2D::Shutdown()
	{
		GBC_PROFILE_FUNCTION();

		delete[] data.localQuadVertexBufferStart;
		delete[] data.localCircleVertexBufferStart;
		delete[] data.textures;

		// Make sure to free GPU memory
		data.quadShader.reset();
		data.quadVertexArray.reset();
		data.quadVertexBuffer.reset();
		data.quadIndexBuffer.reset();

		data.circleShader.reset();
		data.circleVertexArray.reset();
		data.circleVertexBuffer.reset();
	}

	void Renderer2D::BeginScene(const glm::mat4& viewProjection)
	{
		GBC_PROFILE_FUNCTION();

		Renderer::CameraBuffer cameraBuffer;
		cameraBuffer.viewProjection = viewProjection;
		Renderer::UploadCameraBuffer(cameraBuffer);
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::Flush()
	{
		GBC_PROFILE_FUNCTION();

		FlushQuads();
		FlushCircles();
		ResetQuads();
		ResetCircles();
	}

	void Renderer2D::FlushQuads()
	{
		uint32_t quadVertexBufferSize = static_cast<uint32_t>((data.quadCount * (4 * sizeof(QuadVertex))));
		if (quadVertexBufferSize != 0)
		{
			// Copy quad vertices to GPU
			data.quadVertexBuffer->SetData(data.localQuadVertexBufferStart, quadVertexBufferSize);

			// Bind textures
			for (uint32_t i = 0; i < data.textureCount; i++)
				data.textures[i]->Bind(i);

			// Actually render
			data.quadShader->Bind();
			Renderer::DrawIndexed(data.quadVertexArray, data.quadIndexBuffer, 0, data.quadCount * 6);

#if GBC_ENABLE_STATS
			data.statistics.drawCallCount++;
#endif
		}
	}

	void Renderer2D::FlushCircles()
	{
		uint32_t circleVertexBufferSize = static_cast<uint32_t>((data.circleCount * (4 * sizeof(CircleVertex))));
		if (circleVertexBufferSize != 0)
		{
			// Copy circle vertices to GPU
			data.circleVertexBuffer->SetData(data.localCircleVertexBufferStart, circleVertexBufferSize);

			// Actually render
			data.circleShader->Bind();

			// Yes, it is intentional to use the quad index buffer here because all the indices will be the
			// same anyway, so there's no point in creating the same data twice for both the quads and circles.
			Renderer::DrawIndexed(data.circleVertexArray, data.quadIndexBuffer, 0, data.circleCount * 6);

#if GBC_ENABLE_STATS
			data.statistics.drawCallCount++;
#endif
		}
	}

	void Renderer2D::ResetQuads()
	{
		data.quadCount = 0;
		data.localQuadVertexBufferCurrent = data.localQuadVertexBufferStart;

		// Remove the references once the textures has been rendered
		for (uint32_t i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	void Renderer2D::ResetCircles()
	{
		data.circleCount = 0;
		data.localCircleVertexBufferCurrent = data.localCircleVertexBufferStart;
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

	void Renderer2D::EnsureQuadBatch(uint32_t texIndex)
	{
		if (data.quadCount >= data.maxQuads || texIndex >= data.maxTextures)
			EndScene();
	}

	void Renderer2D::EnsureCircleBatch()
	{
		if (data.circleCount >= data.maxQuads)
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
		EnsureQuadBatch(texIndex);
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
		for (uint32_t i = 0; i < data.quadVertexCount; i++, data.localQuadVertexBufferCurrent++)
		{
			data.localQuadVertexBufferCurrent->position = transform * data.quadVertexPositions[i];
			data.localQuadVertexBufferCurrent->tintColor = color;
			data.localQuadVertexBufferCurrent->texCoord = data.quadVertexTexCoords[i];
			data.localQuadVertexBufferCurrent->texIndex = texIndex;
			data.localQuadVertexBufferCurrent->tilingFactor = tilingFactor;
		}

		// Update counts
		data.quadCount++;
#if GBC_ENABLE_STATS
		data.statistics.quadCount++;
#endif
	}

	void Renderer2D::DrawCircle(const glm::vec3& translation, float thickness, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
		DrawCircle(transform, thickness, color);
	}

	void Renderer2D::DrawCircle(const glm::vec3& translation, const glm::vec2& scale, float thickness, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
							* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f));
		DrawCircle(transform, thickness, color);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, float thickness, const glm::vec4& color)
	{
		// The shader can, even if thickness is exactly zero, render pixels at the very edge of the circle.
		if (thickness == 0.0f)
			return;

		EnsureCircleBatch();

		// Handle vertices
		// Again, using the quad vertices here is intentional
		for (uint32_t i = 0; i < data.quadVertexCount; i++, data.localCircleVertexBufferCurrent++)
		{
			data.localCircleVertexBufferCurrent->position = transform * data.quadVertexPositions[i];
			data.localCircleVertexBufferCurrent->localPosition = data.quadVertexPositions[i] * 2.0f;
			data.localCircleVertexBufferCurrent->thickness = thickness;
			data.localCircleVertexBufferCurrent->color = color;
		}

		// Update counts
		data.circleCount++;
#if GBC_ENABLE_STATS
		data.statistics.circleCount++;
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
