#pragma once

#include "gbc.h"
using namespace gbc;

class SandboxLayer : public Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep timestep) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event& event) override;
private:
	bool OnWindowResizeEvent(WindowResizeEvent& event);

	PerspectiveCamera camera;
	PerspectiveCameraController cameraController;
	//Scope<Framebuffer> framebuffer;
	BasicModel model{4, 6}; // Debug
};

// TODO: not in any particular order...
// render one of a framebuffers' color attachments
// renderer statistics + imgui panel for that
// ok so basically make this Hazel except don't focus on the editor https://github.com/TheCherno/Hazel
