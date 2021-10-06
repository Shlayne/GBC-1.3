#pragma once

#include <gbc.h>
#include "GBC/Rendering/EditorCamera.h"
#include <imguizmo/ImGuizmo.h>
#include <filesystem>

namespace gbc
{
	class Panel;
	class ContentBrowserPanel;
	class SceneHierarchyPanel;
	class ScenePropertiesPanel;
	class SceneViewportPanel;
	class TexturePropertiesPanel;

	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep timestep) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnPostImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);
		bool OnKeyPressEvent(KeyPressEvent& event);
		bool OnMouseButtonPressEvent(MouseButtonPressEvent& event);
		bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event);

		// Scene
		void NewScene();
		void OpenScene();
		void OpenSceneFile(const std::filesystem::path& filepath);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		std::filesystem::path sceneFilepath;
		bool hasUnsavedChanges = false;

		enum class SceneState : uint8_t
		{
			Edit = 0, Play = 1
		};
		SceneState sceneState = SceneState::Edit;
		
		EditorCamera editorCamera;
		Ref<Scene> activeScene;
		Ref<Scene> editorScene;
		Ref<Framebuffer> framebuffer;

		Entity selectedEntity;
		Entity selectedEditorEntity;
		ImGuizmo::OPERATION gizmoType = ImGuizmo::OPERATION::NONE;
		bool canUseGizmos = true;
		bool canRenderGizmos = true;

		// Panels
		void UI_Dockspace();
		void UI_MenuBar();
		void UI_ToolBar();

		template<typename T> T* AddPanel(const std::string& name);

		std::vector<Panel*> panels;

		friend class ContentBrowserPanel;
		friend class SceneHierarchyPanel;
		friend class ScenePropertiesPanel;
		friend class SceneViewportPanel;
		friend class TexturePropertiesPanel;

		ContentBrowserPanel* contentBrowserPanel = nullptr;
		SceneHierarchyPanel* sceneHierarchyPanel = nullptr;
		ScenePropertiesPanel* scenePropertiesPanel = nullptr;
		SceneViewportPanel* sceneViewportPanel = nullptr;
		TexturePropertiesPanel* texturePropertiesPanel = nullptr;

		// Resources
		Ref<Texture2D> playButtonTexture;
		Ref<Texture2D> stopButtonTexture;
	};
}

#include "EditorLayer.inl"
