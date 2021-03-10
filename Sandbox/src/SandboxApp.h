#pragma once

#include "gbc.h"
using namespace gbc;

class SandboxApp : public Application
{
public:
	SandboxApp();
	~SandboxApp();

	virtual void OnClientUpdate(Timestep timestep) override;
	virtual void OnClientRender() override;
	virtual void OnClientImGuiRender() override;
	virtual void OnClientEvent(Event& event) override;
private:
	PerspectiveCamera camera;
	PerspectiveCameraController cameraController;
	//Scope<Framebuffer> framebuffer;
	BasicModel model{4, 6}; // Debug
};

// TODO: not in any particular order...
// render one of a framebuffers' color attachments
// profiler / renderer statistics + imgui panel for that
// ok so basically make this Hazel except don't focus on the editor https://github.com/TheCherno/Hazel

