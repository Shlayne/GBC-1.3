#pragma once

#include "Events/Event.h"
#include "Util/TextureAtlas.h"
#include "Util/Camera.h"
#include "Util/CameraController.h"

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
	CameraController cameraController;
};
