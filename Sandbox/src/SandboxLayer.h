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
	Scope<Scene> scene;

#if GBC_ENABLE_STATS
	BasicRenderer::Statistics statistics;
#endif
};
