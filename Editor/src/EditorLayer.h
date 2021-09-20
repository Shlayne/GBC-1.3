#pragma once

#include <gbc.h>
#include <map>
#include <filesystem>
#include <imguizmo/ImGuizmo.h>
#include "GBC/Rendering/EditorCamera.h"
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
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnKeyPressEvent(KeyPressEvent& event);
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);

		// Scene
		void OnScenePlay();
		void OnSceneStop();
		void ClearScene();

		void NewScene();
		void OpenScene();
		void OpenSceneFile(const std::filesystem::path& filepath);
		void SaveScene();
		void SaveAsScene();

		std::filesystem::path currentFilepath;
		bool hasUnsavedChanges = false;

		enum class SceneState : uint8_t
		{
			Edit = 0, Play = 1
		};
		SceneState sceneState = SceneState::Edit;
		
		EditorCamera editorCamera;
		Ref<Scene> scene;
		Ref<Framebuffer> framebuffer;

		Entity selectedEntity;
		ImGuizmo::OPERATION gizmoType = ImGuizmo::OPERATION::NONE;
		bool canUseGizmos = true;
		bool canRenderGizmos = true;

		// Panels
		void UI_Dockspace();
		void UI_MenuBar();
		void UI_ToolBar();

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

		// Resources
		Ref<Texture2D> playButtonTexture;
		Ref<Texture2D> stopButtonTexture;
	};
}
