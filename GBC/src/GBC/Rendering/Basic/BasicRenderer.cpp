#include "gbcpch.h"
#include "BasicRenderer.h"
#include "GBC/IO/FileIO.h"
#include "GBC/Rendering/Renderer.h"
#include "GBC/Rendering/Shader.h"

namespace gbc
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 tintColor;
		glm::vec2 texCoord;
		unsigned int texIndex;
	};

	struct BasicRendererData
	{
		Ref<Shader> shader;
		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<IndexBuffer> indexBuffer;

		unsigned int localIndexCount = 0;
		unsigned int* localIndexBufferStart = nullptr;
		unsigned int* localIndexBufferCurrent = nullptr;
		unsigned int localVertexCount = 0;
		Vertex* localVertexBufferStart = nullptr;
		Vertex* localVertexBufferCurrent = nullptr;

		Ref<Texture>* textures = nullptr;
		unsigned int textureCount = 1;

		unsigned int maxVertices = 0;
		unsigned int maxIndices = 0;
		unsigned int maxTextures = 0;

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
		data.maxTextures = (unsigned int)Renderer::GetMaxTextureSlots();
		data.textures = new Ref<Texture>[data.maxTextures];

		// Setup local buffers
		data.maxVertices = 900000; // TODO: query drivers
		data.maxIndices  = 600000; // TODO: query drivers
		data.localVertexBufferStart = new Vertex[data.maxVertices];
		data.localVertexBufferCurrent = data.localVertexBufferStart;
		data.localIndexBufferStart = new unsigned int[data.maxIndices];
		data.localIndexBufferCurrent = data.localIndexBufferStart;

		// Setup internal buffers
		data.vertexBuffer = VertexBuffer::CreateRef(data.maxVertices * sizeof(Vertex), nullptr, BufferUsage::DynamicDraw);
		data.vertexBuffer->SetLayout({
			{BufferElementType::Float3, "position" },
			{BufferElementType::Float4, "tintColor"},
			{BufferElementType::Float2, "texCoord" },
			{BufferElementType::UInt,   "texIndex" }
		});

		data.vertexArray = VertexArray::CreateRef();
		data.vertexArray->AddVertexBuffer(data.vertexBuffer);

		data.indexBuffer = IndexBuffer::CreateRef(data.maxIndices, nullptr, BufferUsage::DynamicDraw);

		// Setup shader
		data.shader = Shader::CreateRef("resources/shaders/BasicShader.glsl");
		data.shader->Bind();
		int* samplers = new int[data.maxTextures];
		for (unsigned int i = 0; i < data.maxTextures; i++)
			samplers[i] = i;
		data.shader->SetUniforms("textures", samplers, data.maxTextures);
		delete[] samplers;

		// Setup white texture
		Ref<LocalTexture2D> whiteTexture = CreateRef<LocalTexture2D>(1, 1, 4);
		*(unsigned int*)whiteTexture->GetData() = 0xffffffff;
		data.textures[0] = Texture::CreateRef(whiteTexture);
	}

	void BasicRenderer::Shutdown()
	{
		delete[] data.localVertexBufferStart;
		delete[] data.localIndexBufferStart;
		delete[] data.textures;
	}

	void BasicRenderer::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		// Set shader uniforms
		data.shader->Bind();
		data.shader->SetUniform("projection", camera.GetProjection());
		data.shader->SetUniform("cameraTransform", transform);
	}

	void BasicRenderer::EndScene()
	{
		unsigned int vertexBufferSize = (unsigned int)(data.localVertexCount * sizeof(Vertex));
		unsigned int indexBufferSize = (unsigned int)(data.localIndexCount * sizeof(unsigned int));

		if (vertexBufferSize != 0 && indexBufferSize != 0)
		{
			// Copy local buffers to internal buffers
			data.vertexBuffer->SetData(vertexBufferSize, data.localVertexBufferStart);
			data.indexBuffer->SetData(indexBufferSize, data.localIndexBufferStart);

			// Bind renderables
			for (unsigned int i = 0; i < data.textureCount; i++)
				data.textures[i]->Bind(i);

			// Actually render
			Renderer::DrawIndexed(data.vertexArray, data.indexBuffer, data.localIndexCount);
			Reset();

#if GBC_ENABLE_STATS
			data.statistics.drawCalls++;
#endif
		}
	}

	void BasicRenderer::EnsureBatch(unsigned int vertexCount, unsigned int indexCount, unsigned int texIndex)
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

		// Remove the references once the renderables has been rendered
		for (unsigned int i = 1; i < data.textureCount; i++)
			data.textures[i] = nullptr;
		data.textureCount = 1;
	}

	unsigned int BasicRenderer::GetTexIndex(const Ref<Texture>& texture)
	{
		if (!texture)
			return 0;

		for (unsigned int i = 1; i < data.textureCount; i++)
			if (*data.textures[i]->GetTexture() == *texture->GetTexture())
				return i;

		return data.textureCount;
	}

	void BasicRenderer::Submit(const Ref<BasicMesh>& mesh, const glm::mat4& transform, const RenderableComponent& renderableComponent)
	{
		if (mesh == nullptr)
			return;

		// Handle renderable
		auto& texture = renderableComponent.texture;
		unsigned int texIndex = GetTexIndex(texture);
		EnsureBatch(mesh->vertexCount, mesh->indexCount, texIndex);
		if (texIndex >= data.textureCount)
		{
			data.textures[texIndex = data.textureCount++] = texture;
#if GBC_ENABLE_STATS
			data.statistics.textureCount++;
#endif
		}

		// Handle vertices
		for (unsigned int i = 0; i < mesh->vertexCount; i++, data.localVertexBufferCurrent++)
		{
			data.localVertexBufferCurrent->position = transform * glm::vec4(mesh->vertices[i].position, 1.0f);
			data.localVertexBufferCurrent->texCoord = mesh->vertices[i].texCoord;
			data.localVertexBufferCurrent->tintColor = mesh->vertices[i].tintColor * renderableComponent.tintColor;
			data.localVertexBufferCurrent->texIndex = texIndex;
		}

		// Handle indices
		for (unsigned int i = 0; i < mesh->indexCount; i++, data.localIndexBufferCurrent++)
			*data.localIndexBufferCurrent = data.localVertexCount + mesh->indices[i];

		// Update local counts
		data.localVertexCount += mesh->vertexCount;
		data.localIndexCount += mesh->indexCount;

#if GBC_ENABLE_STATS
		data.statistics.indexCount += mesh->indexCount;
		data.statistics.vertexCount += mesh->vertexCount;
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
