#pragma once

#include "gbc.h"
#include "GBC/Rendering/EditorCamera.h"
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
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnKeyPressEvent(KeyPressEvent& event);

		void ClearScene();

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveAsScene();

		std::string currentFilepath;
		bool hasUnsavedChanges = false;
		
		EditorCamera editorCamera;
		Ref<Scene> scene;
		Ref<Framebuffer> framebuffer;

		Entity selectedEntity;
		int gizmoType = -1;

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
// 1) Use "#if GBC_PROJECT_EDITOR" for editor-only code, e.g. entityIDs in BasicRender::Vertex

// https://www.youtube.com/watch?v=f-GbHye1VFQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=96
