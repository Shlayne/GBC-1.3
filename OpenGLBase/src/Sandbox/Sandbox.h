#pragma once

#include "Events/Event.h"
#include "Util/TextureAtlas.h"
#include "Util/PerspectiveCamera.h"
#include "Util/CameraController3D.h"
#include "Rendering/Basic/BasicModel.h"
//#include "Rendering/Framebuffer.h"

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
	CameraController3D cameraController;
	//std::unique_ptr<Framebuffer> framebuffer;
	BasicModel model{4, 6}; // Debug
};

// TODO: not in any particular order...
// 1) texture specifications
// 2) orthographic camera
// 3) isometric camera ???
// 4) render one of a framebuffers' color attachments
// 5) logging
// 6) precompiled headers
