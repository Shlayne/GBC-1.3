#include "Sandbox.h"
#include "Rendering/Basic/BasicRenderer.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"
#include "Core/Application.h"
#include <iostream>
#include "imgui/imgui.h"

Sandbox::Sandbox()
	: camera(glm::radians(90.0f), 0.01f, 1000.0f)
{
	model.vertices[0] = {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	model.vertices[1] = {{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	model.vertices[2] = {{ 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	model.vertices[3] = {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	model.indices[0] = 0;
	model.indices[1] = 1;
	model.indices[2] = 2;
	model.indices[3] = 2;
	model.indices[4] = 3;
	model.indices[5] = 0;

	cameraController.setTranslation({0.0f, 0.0f, 1.0f});

	Window& window = Application::get().getWindow();
	camera.onViewportResize(window.getWidth(), window.getHeight());

	//FramebufferSpecification framebufferSpecification;
	//framebufferSpecification.attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth};
	//framebufferSpecification.width = window.getWidth();
	//framebufferSpecification.height = window.getHeight();
	//framebuffer.reset(new Framebuffer(framebufferSpecification));
}

Sandbox::~Sandbox()
{

}

void Sandbox::onUpdate(float deltaTime)
{
	cameraController.onUpdate(deltaTime);
}

void Sandbox::onRender()
{
	//framebuffer->Bind();
	
	Renderer::setClearColor({0.2f, 0.3f, 0.9f, 1.0f});
	Renderer::clear();

	BasicRenderer::beginScene(cameraController.getTransform(), camera.getProjection());

	// Render here
	BasicRenderer::submit(model);

	BasicRenderer::endScene();

	//framebuffer->Unbind();
}

void Sandbox::onImGuiRender()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//ImGui::Begin("FramebufferTest");
	//ImGui::Image((void*)((size_t)framebuffer->GetColorAttachment()), ImVec2((float)framebuffer->GetSpecification().width, (float)framebuffer->GetSpecification().height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	//ImGui::End();
	//ImGui::PopStyleVar();
}

void Sandbox::onEvent(const Event& event)
{
	cameraController.onEvent(event);

	switch (event.getType())
	{
		case EventType::WindowResize:
		{
			const WindowResizeEvent& wre = (const WindowResizeEvent&)event;
			camera.onViewportResize(wre.getWidth(), wre.getHeight());
			break;
		}
	}
}
