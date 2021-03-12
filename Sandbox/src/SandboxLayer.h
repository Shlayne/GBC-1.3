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
#if GBC_ENABLE_IMGUI
	virtual void OnImGuiRender() override;
#endif
	virtual void OnEvent(Event& event) override;
private:
	bool OnWindowResizeEvent(WindowResizeEvent& event);

	// TODO: remove
	PerspectiveCamera camera;
	PerspectiveCameraController cameraController;

	Scope<Scene> scene;

	// Debug
	BasicModel model{4, 6};
	BasicRenderable renderable;

#if GBC_ENABLE_STATS
	BasicRenderer::Statistics statistics;
#endif
};

// https://www.youtube.com/watch?v=GfSzeAcsBb0&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=80
