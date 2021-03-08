#pragma once

#include "Events/Event.h"
#include "Util/TextureAtlas.h"
#include "Util/Camera.h"
#include "Util/CameraController3D.h"
#include "Rendering/Basic/BasicModel.h"
//#include "Rendering/Framebuffer.h"

class Sandbox
{
public:
	Sandbox();
	~Sandbox();

	void onUpdate(float deltaTime);
	void onRender();
	void onImGuiRender();
	void onEvent(const Event& event);
private:
	Camera camera;
	CameraController3D cameraController;
	//std::unique_ptr<Framebuffer> framebuffer;
	BasicModel model{4, 6};
};

// TODO: texture specifications
