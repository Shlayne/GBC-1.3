#pragma once

#include "Events/Event.h"
#include "Util/TextureAtlas.h"
#include "Util/PerspectiveCamera.h"
#include "Util/PerspectiveCameraController.h"
#include "Rendering/Basic/BasicModel.h"
//#include "Rendering/Framebuffer.h"

using namespace cbc;

class Sandbox
{
public:
	Sandbox();
	~Sandbox();

	void OnUpdate(float deltaTime);
	void OnRender();
	void OnImGuiRender();
	void OnEvent(Event& event);
private:
	PerspectiveCamera camera;
	PerspectiveCameraController cameraController;
	//Scope<Framebuffer> framebuffer;
	BasicModel model{4, 6}; // Debug
};

// TODO: not in any particular order...
// render one of a framebuffers' color attachments
// logging
// precompiled headers
// profiler / renderer statistics + imgui panel for that
// premake
// ok so basically make this Hazel except don't focus on the editor https://github.com/TheCherno/Hazel
// rename the git repository to C++BaseCode
