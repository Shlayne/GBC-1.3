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

	BasicRenderable::BasicRenderable(const Ref<Texture>& texture)
		: type(Type::Texture), texture(texture) {}
	BasicRenderable::BasicRenderable(const Ref<Framebuffer>& framebuffer, unsigned int attachmentIndex)
		: type(Type::Framebuffer), framebuffer(framebuffer), attachmentIndex(attachmentIndex) {}

	void BasicRenderable::Bind(unsigned int slot)
	{
		switch (type)
		{
			case Type::Texture:
				(*texture)->Bind(slot);
				return;
			case Type::Framebuffer:
				(*framebuffer)->BindColorTexture(attachmentIndex, slot);
				return;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
	}

	void BasicRenderable::Unbind(unsigned int slot)
	{
		switch (type)
		{
			case Type::Texture:
				(*texture)->Unbind(slot);
				return;
			case Type::Framebuffer:
				(*framebuffer)->UnbindColorTexture(slot);
				return;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
	}

	void BasicRenderable::Clear()
	{
		switch (type)
		{
			case Type::Texture:     texture = nullptr; break;
			case Type::Framebuffer: framebuffer = nullptr; break;
			case Type::None: return;
		}
		type = Type::None;
	}

	BasicRenderable::operator bool() const
	{
		switch (type)
		{
			case Type::None:        return false;
			case Type::Texture:     return texture->operator bool();
			case Type::Framebuffer: return framebuffer->operator bool();
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
		return false;
	}

	bool BasicRenderable::operator==(const BasicRenderable& renderable) const
	{
		if (type != renderable.type || type == Type::None)
			return false;

		switch (type)
		{
			case Type::Texture:     return *texture == renderable.texture;
			case Type::Framebuffer: return *framebuffer == renderable.framebuffer;
		}

		GBC_CORE_ASSERT(false, "Unknown Renderable Type!");
		return false;
	}

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

		BasicRenderable* renderables = nullptr;
		unsigned int renderableCount = 1;

		unsigned int maxVertices = 0;
		unsigned int maxIndices = 0;
		unsigned int maxRenderables = 0;

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
		data.maxRenderables = (unsigned int)Renderer::GetMaxTextureSlots();
		data.renderables = new BasicRenderable[data.maxRenderables];

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
		int* samplers = new int[data.maxRenderables];
		for (unsigned int i = 0; i < data.maxRenderables; i++)
			samplers[i] = i;
		data.shader->SetUniforms("textures", samplers, data.maxRenderables);
		delete[] samplers;

		// Setup white texture
		Ref<LocalTexture2D> whiteTexture = CreateRef<LocalTexture2D>(1, 1, 4);
		*(unsigned int*)whiteTexture->GetData() = 0xffffffff;
		data.renderables[0] = Texture::CreateRef(whiteTexture);
	}

	void BasicRenderer::Shutdown()
	{
		delete[] data.localVertexBufferStart;
		delete[] data.localIndexBufferStart;
		delete[] data.renderables;
	}

	void BasicRenderer::BeginScene(const glm::mat4& cameraTransform, const glm::mat4& projection)
	{
		// Set shader uniforms
		data.shader->Bind();
		data.shader->SetUniform("projection", projection);
		data.shader->SetUniform("cameraTransform", cameraTransform);
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
			for (unsigned int i = 0; i < data.renderableCount; i++)
				data.renderables[i].Bind(i);

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
			texIndex >= data.maxRenderables)
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
		for (unsigned int i = 1; i < data.renderableCount; i++)
			data.renderables[i].Clear();
		data.renderableCount = 1;
	}

	unsigned int BasicRenderer::GetTexIndex(const BasicRenderable& renderable)
	{
		if (!renderable)
			return 0;

		for (unsigned int i = 1; i < data.renderableCount; i++)
			if (data.renderables[i] == renderable)
				return i;

		return data.renderableCount;
	}

	void BasicRenderer::Submit(const BasicModel& model, const glm::mat4& transform, const BasicRenderable& renderable)
	{
		// Handle renderable
		unsigned int texIndex = GetTexIndex(renderable);
		EnsureBatch(model.vertexCount, model.indexCount, texIndex);
		if (texIndex >= data.renderableCount)
		{
			data.renderables[texIndex = data.renderableCount++] = renderable;
#if GBC_ENABLE_STATS
			data.statistics.textureCount++;
#endif
		}

		// Handle vertices
		for (unsigned int i = 0; i < model.vertexCount; i++, data.localVertexBufferCurrent++)
		{
			data.localVertexBufferCurrent->position = transform * glm::vec4(model.vertices[i].position, 1.0f);
			data.localVertexBufferCurrent->texCoord = model.vertices[i].texCoord;
			data.localVertexBufferCurrent->tintColor = model.vertices[i].tintColor;
			data.localVertexBufferCurrent->texIndex = texIndex;
		}

		// Handle indices
		for (unsigned int i = 0; i < model.indexCount; i++, data.localIndexBufferCurrent++)
			*data.localIndexBufferCurrent = data.localVertexCount + model.indices[i];

		// Update local counts
		data.localVertexCount += model.vertexCount;
		data.localIndexCount += model.indexCount;

#if GBC_ENABLE_STATS
		data.statistics.indexCount += model.indexCount;
		data.statistics.vertexCount += model.vertexCount;
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
