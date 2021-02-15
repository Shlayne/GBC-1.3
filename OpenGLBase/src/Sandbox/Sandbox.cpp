#include "Sandbox.h"

#include "Rendering/Basic/BasicRenderer.h"
#include "Events/WindowEvents.h"
#include "Core/Application.h"

Sandbox::Sandbox()
	: camera(glm::radians(90.0f), 0.01f, 1000.0f)
{
	cameraController.setTranslation({32, 48, 32});

	Window& window = Application::get().getWindow();
	camera.onViewportResize(window.getWidth(), window.getHeight());
}

Sandbox::~Sandbox()
{

}

void Sandbox::onUpdate(float deltaTime)
{
	// TODO: camera controller should be attached to a player entity
	// entities should be owned by the world, so this would eventually be called somewhere in world->onUpdate(deltaTime)
	cameraController.onUpdate(deltaTime);
}

void Sandbox::onRender()
{
	Renderer::clear();
	Renderer::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});

	BasicRenderer::beginScene(cameraController.getTransform(), camera.getProjection());
	// Render here
	BasicRenderer::submit();
	BasicRenderer::endScene();
}

void Sandbox::onImGuiRender()
{
	
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
