#include "EditorLayer.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/Scene/SceneSerializer.h"
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Panels/Dist/ContentBrowserPanel.h"
#include "Panels/Dist/SceneHierarchyPanel.h"
#include "Panels/Dist/ScenePropertiesPanel.h"
#include "Panels/Dist/SceneViewportPanel.h"
#include "Panels/Dist/TexturePropertiesPanel.h"
#if GBC_ENABLE_STATS
	#include "Panels/Release/RendererInfoPanel.h"
	#include "Panels/Release/StasticsPanel.h"
#endif
#if GBC_ENABLE_PROFILE_RUNTIME
	#include "Panels/Debug/ProfilingPanel.h"
#endif
#if GBC_CONFIG_DEBUG
	#include "Panels/Debug/DemoPanel.h"
#endif

namespace gbc
{
	void EditorLayer::OnAttach()
	{
		GBC_PROFILE_FUNCTION();

		auto& application = Application::Get();
		auto& window = application.GetWindow();
		auto& assetManager = application.GetAssetManager();

		playButtonTexture = assetManager.GetOrLoadTexture(L"Resources/Icons/PlayButton.png");
		stopButtonTexture = assetManager.GetOrLoadTexture(L"Resources/Icons/StopButton.png");

		FramebufferSpecification framebufferSpecification;
		framebufferSpecification.width = window.GetWidth();
		framebufferSpecification.height = window.GetHeight();
		framebufferSpecification.attachments = { { FramebufferTextureFormat::RGBA8 }, { FramebufferTextureFormat::Depth24Stencil8 } };
		framebuffer = Framebuffer::Create(framebufferSpecification);

		editorScene = CreateRef<Scene>();
		activeScene = editorScene;

		// The panels are weirdly ordered because ImGui has a bug that orders them incorrectly and has for over 2 years now, sooo.......
#if GBC_CONFIG_DEBUG
		AddPanel<DemoPanel>("Demo");
#endif
#if GBC_ENABLE_PROFILE_RUNTIME
		AddPanel<ProfilingPanel>("Profiling");
#endif
#if GBC_ENABLE_STATS
		AddPanel<RendererInfoPanel>("Renderer Info");
		AddPanel<StatisticsPanel>("Statistics");
#endif
		contentBrowserPanel = AddPanel<ContentBrowserPanel>("Content Browser");
		sceneHierarchyPanel = AddPanel<SceneHierarchyPanel>("Scene Hierarchy");
		texturePropertiesPanel = AddPanel<TexturePropertiesPanel>("Texture Properties");
		scenePropertiesPanel = AddPanel<ScenePropertiesPanel>("Scene Properties");

		sceneViewportPanel = AddPanel<SceneViewportPanel>("Scene Viewport");
	}

	void EditorLayer::OnDetach()
	{
		GBC_PROFILE_FUNCTION();

		for (auto panel : panels)
			delete panel;
	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		bool viewportFocused = sceneViewportPanel->IsFocused();
		bool viewportHovered = sceneViewportPanel->IsHovered();

		editorCamera.SetBlocked(ImGuizmo::IsUsing());
		Application::Get().GetImGuiWrapper().SetBlockEvents(!viewportFocused && !viewportHovered);

		if (sceneViewportPanel->HasSizeChanged())
		{
			const glm::ivec2& viewportSize = sceneViewportPanel->GetSize();

			editorCamera.OnViewportResize(viewportSize.x, viewportSize.y);
			framebuffer->OnViewportResize(viewportSize.x, viewportSize.y);
			activeScene->OnViewportResize(viewportSize.x, viewportSize.y);
		}

		switch (sceneState)
		{
			case SceneState::Edit:
				editorCamera.OnUpdate(viewportHovered ? timestep : Timestep());
				break;
			case SceneState::Play:
				activeScene->OnRuntimeUpdate(timestep);
				break;
		}
	}

	void EditorLayer::OnRender()
	{
		GBC_PROFILE_FUNCTION();

#if GBC_ENABLE_STATS
		Renderer2D::ResetStatistics();
		Renderer3D::ResetStatistics();
#endif

		framebuffer->Bind();

		Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		Renderer::Clear();

		switch (sceneState)
		{
			case SceneState::Edit:
				activeScene->OnEditorRender(editorCamera);
				break;
			case SceneState::Play:
				activeScene->OnRuntimeRender();
				break;
		}

		framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		GBC_PROFILE_FUNCTION();

		UI_Dockspace();

		for (auto panel : panels)
			panel->OnImGuiRender();
	}

	void EditorLayer::OnPostImGuiRender()
	{
		if (contentBrowserPanel->IsDragSelectActive())
		{
			Window& window = Application::Get().GetWindow();

			Renderer::ClearDepthOnly();
			Renderer::SetViewport(0, 0, window.GetWidth(), window.GetHeight());
			float windowWidth = static_cast<float>(window.GetWidth());
			float windowHeight = static_cast<float>(window.GetHeight());

			float aspect = windowWidth / windowHeight;
			Renderer2D::BeginScene(glm::ortho(-aspect * 0.5f, aspect * 0.5f, -0.5f, 0.5f));

			glm::ivec2 topLeft, bottomRight;
			contentBrowserPanel->GetDragSelectBounds(topLeft, bottomRight);

			glm::vec2 size = glm::vec2(bottomRight - topLeft - 1) / windowHeight;
			glm::vec2 outlineSize = glm::vec2(bottomRight - topLeft + 1) / windowHeight;
			glm::vec2 center = (glm::vec2(topLeft + bottomRight + 1) / windowHeight - glm::vec2(aspect, 1.0f)) * glm::vec2(0.5f, -0.5f);
			glm::vec2 outlineCenter = (glm::vec2(topLeft + bottomRight + 1) / windowHeight - glm::vec2(aspect, 1.0f)) * glm::vec2(0.5f, -0.5f);

			Renderer2D::DrawQuad(center, size, { 0.102f, 0.337f, 0.608f, 0.400f });
			Renderer2D::DrawQuad(outlineCenter, outlineSize, { 0.260f, 0.590f, 0.980f, 1.000f }); // TODO: drawing lines

			Renderer2D::EndScene();
		}
	}

	void EditorLayer::UI_Dockspace()
	{
		ImGuiWindowFlags dockspaceFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::Begin("Dockspace", nullptr, dockspaceFlags);
		ImGui::PopStyleVar(2);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, { 200.0f, ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f });
		ImGui::DockSpace(ImGui::GetID("Dockspace"));
		ImGui::PopStyleVar();

		UI_MenuBar();
		ImGui::End();
	}

	void EditorLayer::UI_MenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();
				if (ImGui::MenuItem("Save...", "Ctrl+S"))
					SaveScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::BeginMenu("Panels"))
				{
					for (auto panel : panels)
					{
						bool enabled = panel->IsEnabled();
						if (ImGui::MenuItem(panel->GetName().c_str(), nullptr, &enabled))
							panel->ToggleEnabled();
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			UI_ToolBar();

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::UI_ToolBar()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		auto& colors = style.Colors;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0.0f, 0.0f });

		float buttonSize = ImGui::GetItemRectSize().y + 4.0f;
		Ref<Texture2D> icon = sceneState == SceneState::Edit ? playButtonTexture : stopButtonTexture;

		ImGui::SetCursorPos({ (ImGui::GetWindowContentRegionWidth() - buttonSize) * 0.5f, 0.0f });
		if (ImGui::ImageButton((void*)(size_t)icon->GetRendererID(), { buttonSize, buttonSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0))
		{
			switch (sceneState)
			{
				case SceneState::Edit:
					OnScenePlay();
					break;
				case SceneState::Play:
					OnSceneStop();
					break;
			}
		}

		ImGui::PopStyleVar(2);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		GBC_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch(this, &EditorLayer::OnWindowCloseEvent);
		dispatcher.Dispatch(this, &EditorLayer::OnKeyPressEvent);
		dispatcher.Dispatch(this, &EditorLayer::OnMouseButtonReleaseEvent);

		if (!event.handled)
			editorCamera.OnEvent(event);
	}

	bool EditorLayer::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		bool allowedDiscard = true;
		if (hasUnsavedChanges)
		{
			// TODO: Show "Any unsaved work will be lost!" message
			//allowedDiscard = false;
		}

		return !allowedDiscard;
	}

	bool EditorLayer::OnKeyPressEvent(KeyPressEvent& event)
	{
		Mods mods = event.GetMods();
		switch (event.GetKeycode())
		{
			// Scene
			case Keycode::N:
				if (mods.Are(Mods_Control))
				{
					NewScene();
					return true;
				}
				break;
			case Keycode::O:
				if (mods.Are(Mods_Control))
				{
					OpenScene();
					return true;
				}
				break;
			case Keycode::S:
				if (mods.Are(Mods_Control | Mods_Shift))
				{
					SaveSceneAs();
					return true;
				}
				else if (mods.Are(Mods_Control))
				{
					SaveScene();
					return true;
				}
				break;
			case Keycode::Delete:
				if (!mods && selectedEntity && (sceneHierarchyPanel->IsFocused() || sceneViewportPanel->IsFocused()))
				{
					activeScene->DestroyEntity(selectedEntity);
					selectedEntity = {};
					return true;
				}
				break;
			case Keycode::D:
				if (mods.Are(Mods_Control) && selectedEntity && (sceneHierarchyPanel->IsFocused() || sceneViewportPanel->IsFocused()))
				{
					selectedEntity = activeScene->DuplicateEntity(selectedEntity);
					return true;
				}
				break;

			// Gizmos
			case Keycode::Q:
				if (!mods && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::NONE;
					return true;
				}
				break;
			case Keycode::W:
				if (!mods && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::TRANSLATE;
					return true;
				}
				break;
			case Keycode::E:
				if (!mods && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::ROTATE;
					return true;
				}
				break;
			case Keycode::R:
				if (!mods && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::SCALE;
					return true;
				}
				break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event)
	{
		canUseGizmos = true;
		return false;
	}

	void EditorLayer::NewScene()
	{
		bool allowedDiscard = true;
		if (hasUnsavedChanges)
		{
			//bool action;
			//if (ImGuiHelper::ConfirmAction("Create New Scene", &action,
			//	"Are you sure you want to create a new scene?\nAny unsaved work will be lost!"))
			//	allowedDiscard = action;
		}

		if (allowedDiscard)
		{
			if (sceneState != SceneState::Edit)
				OnSceneStop();

			sceneFilepath.clear();

			editorScene = CreateRef<Scene>();
			glm::ivec2 viewportSize = activeScene->GetViewportSize();
			editorScene->OnViewportResize(viewportSize.x, viewportSize.y);
			selectedEntity = {};
			activeScene = editorScene;
		}
	}

	void EditorLayer::OpenScene()
	{
		bool allowedDiscard = true;
		if (hasUnsavedChanges)
		{
			// TODO: Show "Any unsaved work will be lost!" message
			//allowedDiscard = false;
		}

		if (allowedDiscard)
		{
			auto filepath = FileDialog::OpenFile(GetFilter(FileType::scene), projectAssetDirectory);
			if (!filepath.empty())
				OpenSceneFile(filepath);
		}
	}

	void EditorLayer::OpenSceneFile(const std::filesystem::path& filepath)
	{
		if (sceneState != SceneState::Edit)
			OnSceneStop();

		sceneFilepath = filepath;

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(sceneFilepath))
		{
			editorScene = newScene;
			glm::ivec2 viewportSize = activeScene->GetViewportSize();
			editorScene->OnViewportResize(viewportSize.x, viewportSize.y);
			selectedEntity = {};
			activeScene = editorScene;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!sceneFilepath.empty())
		{
			hasUnsavedChanges = false;

			SceneSerializer serializer(activeScene);
			serializer.Serialize(sceneFilepath);
		}
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		auto filepath = FileDialog::SaveFile(GetFilter(FileType::scene), projectAssetDirectory);
		if (!filepath.empty())
		{
			// Add extension to extensionless path
			if (!IsSceneFilepath(filepath))
				AppendSceneTypeTo(filepath);

			sceneFilepath = filepath;
			SaveScene();
		}
	}

	void EditorLayer::OnScenePlay()
	{
		sceneState = SceneState::Play;
		canRenderGizmos = false;
		ImGuiHelper::SetScenePlayColors();

		activeScene = Scene::Copy(editorScene);
		activeScene->OnRuntimePlay();

		selectedEditorEntity = selectedEntity;
		selectedEntity = selectedEntity ? activeScene->GetExistingEntity(selectedEntity.GetUUID()) : Entity{};
	}

	void EditorLayer::OnSceneStop()
	{
		sceneState = SceneState::Edit;
		canRenderGizmos = true;
		ImGuiHelper::SetSceneEditColors();
		activeScene->OnRuntimeStop();
		activeScene = editorScene;

		selectedEntity = selectedEditorEntity;
	}
}
