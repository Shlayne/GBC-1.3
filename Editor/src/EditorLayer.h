#pragma once

#include "gbc.h"
#include "GBC/Rendering/EditorCamera.h"
#include <map>
#include "Panels/SceneViewportPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ScenePropertiesPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);

		void ClearScene();

		void NewScene();
		void OpenScene();
		void OpenSceneFile(const std::string& filepath);
		void SaveScene();
		void SaveAsScene();

		std::string currentFilepath;
		bool hasUnsavedChanges = false;
		
		EditorCamera editorCamera;
		Ref<Scene> scene;
		Ref<Framebuffer> framebuffer;

		Entity selectedEntity;
		int gizmoType = -1;
		bool canUseGizmos = true;

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
		ContentBrowserPanel* contentBrowserPanel = nullptr;
	};
}

// TODO:
// 1) Scene base class, then EditorScene and RuntimeScene inherit from Scene
// 
// BUGS:
// 1) resizing main window with short panels at the bottom causes tables
// to have zero or negative size, which makes imgui assert

// Waiting for episode 100 https://www.youtube.com/watch?v=CU9v3uUgRaE&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=99
