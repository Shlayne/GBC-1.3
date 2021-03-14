#pragma once

#include "gbc.h"
#include "Panels/SceneViewportPanel.h"
#include <map>

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
		Ref<Framebuffer> framebuffer;

		// Panels
		template<typename T, typename... Args>
		T* AddPanel(const std::string& name, Args&&... args)
		{
			T* panel = new T(name, std::forward<Args>(args)...);
			panels[name] = panel;
			return panel;
		}

		std::map<std::string, Panel*> panels;
		SceneViewportPanel* sceneViewportPanel = nullptr;
	};
}

// TODO: After resizing viewport, camera controller rotation freaks out because mouse position has changed
// when imgui wrapper was blocking events, so when clicking back into viewport, mouseDPos will be incorrect.
// Same thing when going into and out of fullscreen.
// TLDR: Figure out a way to make mouseDPos 0 on viewport resize.

// https://www.youtube.com/watch?v=ETIhjdVBH-8&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=74
