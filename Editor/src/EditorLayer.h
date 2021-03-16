#pragma once

#include "gbc.h"
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ScenePropertiesPanel.h"
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
		Ref<Scene> scene;
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
		SceneHierarchyPanel* sceneHierarchyPanel = nullptr;
		ScenePropertiesPanel* scenePropertiesPanel = nullptr;
	};
}

// TODO:
// 1) After resizing viewport, camera controller rotation freaks out because mouse position has changed
// when imgui wrapper was blocking events, so when clicking back into viewport, mouseDPos will be incorrect.
// Same thing when going into and out of fullscreen.
// TLDR: Figure out a way to make mouseDPos 0 on viewport/window focus.
// 2) Use "#if GBC_PROJECT_EDITOR" for editor-only code, e.g. entityIDs in BasicRender::Vertex
// 3) BUGS in ScenePropertiesPanel.cpp:
//		a) Tag component text edit box keeps getting deselected
//		b) Cannot undock scene properties panel when selectedEntity is valid
// 4) Editor camera

// https://www.youtube.com/watch?v=zn7N7zHgCcs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=92
