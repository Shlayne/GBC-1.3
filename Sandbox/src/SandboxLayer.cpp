#include "SandboxLayer.h"
//#include "imgui/imgui.h"

void SandboxLayer::OnAttach()
{
	GBC_PROFILE_FUNCTION();

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

void SandboxLayer::OnDetach()
{
	GBC_PROFILE_FUNCTION();
}

void SandboxLayer::OnUpdate(Timestep timestep)
{
	GBC_PROFILE_FUNCTION();

	cameraController.OnUpdate(timestep);
}

void SandboxLayer::OnRender()
{
	GBC_PROFILE_FUNCTION();

	//framebuffer->Bind();
	
	Renderer::SetClearColor({0.2f, 0.3f, 0.9f, 1.0f});
	Renderer::Clear();

	BasicRenderer::BeginScene(cameraController.GetTransform(), camera.GetProjection());

	// Render here
	BasicRenderer::Submit(model);

	BasicRenderer::EndScene();

	//framebuffer->Unbind();
}

void SandboxLayer::OnImGuiRender()
{
	GBC_PROFILE_FUNCTION();

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{});
	//ImGui::Begin("FramebufferTest");
	//ImGui::Image((void*)((size_t)framebuffer->GetColorAttachment()), ImVec2((float)framebuffer->GetSpecification().width, (float)framebuffer->GetSpecification().height), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	//ImGui::End();
	//ImGui::PopStyleVar();
}

void SandboxLayer::OnEvent(Event& event)
{
	cameraController.OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(GBC_BIND_FUNC(SandboxLayer::OnWindowResizeEvent));
}

bool SandboxLayer::OnWindowResizeEvent(WindowResizeEvent& event)
{
	GBC_PROFILE_FUNCTION();

	camera.OnViewportResize(event.GetWidth(), event.GetHeight());
	//framebuffer->OnViewportResize(event.GetWidth(), event.GetHeight());
	return false;
}
