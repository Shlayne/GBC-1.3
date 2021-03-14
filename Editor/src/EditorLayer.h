#pragma once

#include "gbc.h"

namespace gbc
{
	class EditorLayer : public Layer
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
}

// Make the editor because that's important for making a game
// https://www.youtube.com/watch?v=xiSW4UgjLKU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=74
// https://www.youtube.com/watch?v=wziDnE8guvI&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=84
