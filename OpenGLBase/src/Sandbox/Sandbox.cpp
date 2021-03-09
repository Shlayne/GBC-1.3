#include "Sandbox.h"
#include "Rendering/Basic/BasicRenderer.h"
#include "Rendering/Renderer.h"
#include "Events/WindowEvents.h"
#include "Events/MouseEvents.h"
#include "Core/Application.h"
//#include "imgui/imgui.h"

Sandbox::Sandbox()
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

	cameraController.SetTranslation({0.0f, 0.0f, 1.0f});

	Window& window = Application::Get().GetWindow();
	camera.OnViewportResize(window.GetWidth(), window.GetHeight());

	//FramebufferSpecification framebufferSpecification;
	//framebufferSpecification.attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth};
	//framebufferSpecification.width = window.GetWidth();
	//framebufferSpecification.height = window.GetHeight();
	//framebuffer = Framebuffer::CreateScope(framebufferSpecification);
}

Sandbox::~Sandbox()
{

}

void Sandbox::OnUpdate(float deltaTime)
{
	cameraController.OnUpdate(deltaTime);
}

void Sandbox::OnRender()
{
	//framebuffer->Bind();
	
	Renderer::SetClearColor({0.2f, 0.3f, 0.9f, 1.0f});
	Renderer::Clear();

	BasicRenderer::BeginScene(cameraController.GetTransform(), camera.GetProjection());

	// Render here
	BasicRenderer::Submit(model);

	BasicRenderer::EndScene();

	//framebuffer->Unbind();
}

void Sandbox::OnImGuiRender()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	//ImGui::Begin("FramebufferTest");
	//ImGui::Image((void*)((size_t)framebuffer->GetColorAttachment()), ImVec2((float)framebuffer->GetSpecification().width, (float)framebuffer->GetSpecification().height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	//ImGui::End();
	//ImGui::PopStyleVar();
}

void Sandbox::OnEvent(Event& event)
{
	cameraController.OnEvent(event);

	switch (event.GetType())
	{
		case EventType::WindowResize:
		{
			WindowResizeEvent& wre = (WindowResizeEvent&)event;
			camera.OnViewportResize(wre.GetWidth(), wre.GetHeight());
			break;
		}
	}
}
