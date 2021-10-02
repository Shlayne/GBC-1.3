#include "EditorLayer.h"
#include <imgui/imgui.h>
#include "GBC/Scene/SceneSerializer.h"
#include "Panels/ContentBrowserPanel.h"
#if GBC_ENABLE_STATS
	#include "Panels/StasticsPanel.h"
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

		playButtonTexture = Texture2D::Create(CreateRef<LocalTexture2D>("Resources/Icons/PlayButton.png"));
		stopButtonTexture = Texture2D::Create(CreateRef<LocalTexture2D>("Resources/Icons/StopButton.png"));

		Window& window = Application::Get().GetWindow();

		FramebufferSpecification framebufferSpecification;
		framebufferSpecification.width = window.GetWidth();
		framebufferSpecification.height = window.GetHeight();
		framebufferSpecification.attachments = { { FramebufferTextureFormat::RGBA8 }, { FramebufferTextureFormat::Depth24Stencil8 } };
		framebuffer = Framebuffer::Create(framebufferSpecification);

		editorScene = CreateRef<Scene>();
		activeScene = editorScene;

		// TODO: Figure out a different way to have Panels change values in EditorLayer
		sceneViewportPanel = AddPanel<SceneViewportPanel>("Scene Viewport", framebuffer, activeScene, selectedEntity, gizmoType, canUseGizmos, canRenderGizmos, editorCamera, GBC_BIND_FUNC(OpenSceneFile));
		sceneHierarchyPanel = AddPanel<SceneHierarchyPanel>("Scene Hierarchy", activeScene, selectedEntity);
		scenePropertiesPanel = AddPanel<ScenePropertiesPanel>("Scene Properties", selectedEntity);
		contentBrowserPanel = AddPanel<ContentBrowserPanel>("Content Browser");
#if GBC_ENABLE_STATS
		AddPanel<StatisticsPanel>("Statistics", BasicRenderer::GetStatistics());
#endif
#if GBC_ENABLE_PROFILE_RUNTIME
		AddPanel<ProfilingPanel>("Profiling");
#endif
#if GBC_CONFIG_DEBUG
		AddPanel<DemoPanel>("Demo");
#endif
	}

	void EditorLayer::OnDetach()
	{
		GBC_PROFILE_FUNCTION();

		for (auto& [name, panel] : panels)
			delete panel;
	}

	void EditorLayer::OnUpdate(Timestep timestep)
	{
		GBC_PROFILE_FUNCTION();

		bool viewportFocused = sceneViewportPanel->IsFocused();
		bool viewportHovered = sceneViewportPanel->IsHovered();

		editorCamera.SetBlocked(ImGuizmo::IsUsing());
		Application::Get().GetImGuiWrapper().SetBlockEvents(!viewportFocused && !viewportHovered);

		glm::ivec2 viewportPosition = sceneViewportPanel->GetPosition();
		editorCamera.OnViewportMove(viewportPosition.x, viewportPosition.y);

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
		BasicRenderer::ResetStatistics();
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

		for (auto& [name, panel] : panels)
			panel->OnImGuiRender();
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
					for (auto& [name, panel] : panels)
					{
						bool enabled = panel->IsEnabled();
						if (ImGui::MenuItem(name.c_str(), nullptr, &enabled))
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
		dispatcher.Dispatch(this, &EditorLayer::OnMouseButtonPressEvent);
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
		// TODO: assignable keybindings
		bool controlPressed  = Input::IsKeyPressed(Keycode::LeftControl) || Input::IsKeyPressed(Keycode::RightControl);
		bool shiftPressed    = Input::IsKeyPressed(Keycode::LeftShift)   || Input::IsKeyPressed(Keycode::RightShift);
		bool altPressed      = Input::IsKeyPressed(Keycode::LeftAlt)     || Input::IsKeyPressed(Keycode::RightAlt);

		bool none            = !controlPressed && !shiftPressed && !altPressed;
		bool control         =  controlPressed && !shiftPressed && !altPressed;
		bool shift           = !controlPressed &&  shiftPressed && !altPressed;
		bool controlShift    =  controlPressed &&  shiftPressed && !altPressed;
		bool alt             = !controlPressed && !shiftPressed &&  altPressed;
		bool controlAlt      =  controlPressed && !shiftPressed &&  altPressed;
		bool shiftAlt        = !controlPressed &&  shiftPressed &&  altPressed;
		bool controlShiftAlt =  controlPressed &&  shiftPressed &&  altPressed;

		switch (event.GetKeycode())
		{
			// Scene
			case Keycode::N:
				if (control)
				{
					NewScene();
					return true;
				}
				break;
			case Keycode::O:
				if (control)
				{
					OpenScene();
					return true;
				}
				return true;
			case Keycode::S:
				if (control || controlShift)
				{
					if (shiftPressed)
						SaveSceneAs();
					else
						SaveScene();
					return true;
				}
				break;

			// Scene
			case Keycode::Delete:
				if (none && selectedEntity)
				{
					activeScene->DestroyEntity(selectedEntity);
					selectedEntity = {};
				}
				break;
			case Keycode::D:
				if (control && selectedEntity)
					activeScene->DuplicateEntity(selectedEntity);
				break;

			// Gizmos
			case Keycode::Q:
				if (none && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::NONE;
					return true;
				}
				break;
			case Keycode::W:
				if (none && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::TRANSLATE;
					return true;
				}
				break;
			case Keycode::E:
				if (none && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::ROTATE;
					return true;
				}
				break;
			case Keycode::R:
				if (none && sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::SCALE;
					return true;
				}
				break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressEvent(MouseButtonPressEvent& event)
	{
		// TODO: Bring back mouse picking when calculating mesh intersections
		// rather than reading back from a framebuffer attachment

		//if (sceneViewportPanel->IsHovered() && sceneViewportPanel->IsFocused() && event.GetButton() == MouseButton::ButtonLeft && !editorCamera.IsUsing() && !ImGuizmo::IsOver())
		//{
		//	canUseGizmos = false;

		//	glm::ivec2 relativeMousePos = absoluteMousePos - viewportPos;
		//	relativeMousePos.y = viewportSize.y - 1 - relativeMousePos.y;

		//	if (relativeMousePos.x >= 0 && relativeMousePos.x < viewportSize.x &&
		//		relativeMousePos.y >= 0 && relativeMousePos.y < viewportSize.y)
		//	{
		//		int pixel = -1;
		//		framebuffer->Bind();
		//		framebuffer->GetColorPixel(&pixel, relativeMousePos.x, relativeMousePos.y, 1);
		//		framebuffer->Unbind();

		//		selectedEntity = pixel != -1 ? Entity(static_cast<entt::entity>(pixel), scene.get()) : Entity();
		//		return true;
		//	}
		//}

		//canUseGizmos = true;
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
			// TODO: open from assets directory
			auto filepath = FileDialog::OpenFile(L"GBC Scene (*.gscn)\0*.gscn\0", std::filesystem::current_path() /= "Assets");
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
		auto filepath = FileDialog::SaveFile(L"GBC Scene (*.gscn)\0*.gscn\0", std::filesystem::current_path() /= "Assets");
		if (!filepath.empty())
		{
			// Add extension to extensionless path
			if (!filepath.native().ends_with(L".gscn"))
				filepath += L".gscn";

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
