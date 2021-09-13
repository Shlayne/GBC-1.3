#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"
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

		playButtonTexture = Texture2D::CreateRef(CreateRef<LocalTexture2D>("Resources/Icons/PlayButton.png"));
		stopButtonTexture = Texture2D::CreateRef(CreateRef<LocalTexture2D>("Resources/Icons/StopButton.png"));

		Window& window = Application::Get().GetWindow();

		editorCamera = EditorCamera(90.0f, 0.001f, 1000.0f);
		editorCamera.OnViewportResize(window.GetWidth(), window.GetHeight());

		FramebufferSpecification framebufferSpecification;
		framebufferSpecification.width = window.GetWidth();
		framebufferSpecification.height = window.GetHeight();
		framebufferSpecification.attachments = { { FramebufferTextureFormat::RGBA8 }, { FramebufferTextureFormat::Depth24Stencil8 } };
		framebuffer = Framebuffer::CreateRef(framebufferSpecification);

		scene = CreateRef<Scene>();

		// TODO: Figure out a different way to have Panels change values in EditorLayer
		sceneViewportPanel = AddPanel<SceneViewportPanel>("Scene Viewport", framebuffer, scene, selectedEntity, gizmoType, canUseGizmos, canRenderGizmos, editorCamera, GBC_BIND_FUNC(OpenSceneFile));
		sceneHierarchyPanel = AddPanel<SceneHierarchyPanel>("Scene Hierarchy", scene, selectedEntity);
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

		if (sceneViewportPanel->HasSizeChanged())
		{
			const glm::ivec2& viewportSize = sceneViewportPanel->GetSize();

			editorCamera.OnViewportResize(viewportSize.x, viewportSize.y);
			framebuffer->OnViewportResize(viewportSize.x, viewportSize.y);
			scene->OnViewportResize(viewportSize.x, viewportSize.y);
		}

		switch (sceneState)
		{
			case SceneState::Edit:
				editorCamera.OnUpdate(viewportHovered ? timestep : Timestep());
				scene->OnUpdateEditor(timestep);
				break;
			case SceneState::Play:
				scene->OnUpdateRuntime(timestep);
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
				scene->OnRenderEditor(editorCamera);
				break;
			case SceneState::Play:
				scene->OnRenderRuntime();
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
					SaveAsScene();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Alt+F4"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				Window& window = Application::Get().GetWindow();
				if (ImGui::MenuItem(window.IsFullscreen() ? "Windowed" : "Fullscreen"))
					window.ToggleFullscreen();

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

		float buttonSize = ImGui::GetItemRectSize().y;
		Ref<Texture2D> icon = sceneState == SceneState::Edit ? playButtonTexture : stopButtonTexture;

		ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() - buttonSize) * 0.5f, 2.0f));
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
		bool controlPressed = Input::IsKeyPressed(Keycode::LeftControl) || Input::IsKeyPressed(Keycode::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Keycode::LeftShift) || Input::IsKeyPressed(Keycode::RightShift);
		bool altPressed = Input::IsKeyPressed(Keycode::LeftAlt) || Input::IsKeyPressed(Keycode::RightAlt);

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
						SaveAsScene();
					else
						SaveScene();
					return true;
				}
				break;

			// Gizmos
			case Keycode::Q:
				if (sceneViewportPanel->IsFocused())
				{
					gizmoType = -1;
					return true;
				}
				break;
			case Keycode::W:
				if (sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::TRANSLATE;
					return true;
				}
				break;
			case Keycode::E:
				if (sceneViewportPanel->IsFocused())
				{
					gizmoType = ImGuizmo::OPERATION::ROTATE;
					return true;
				}
				break;
			case Keycode::R:
				if (sceneViewportPanel->IsFocused())
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

	void EditorLayer::OnScenePlay()
	{
		sceneState = SceneState::Play;
		canRenderGizmos = false;
		ImGuiHelper::SetScenePlayColors();
		scene->OnPlay();
	}

	void EditorLayer::OnSceneStop()
	{
		sceneState = SceneState::Edit;
		canRenderGizmos = true;
		ImGuiHelper::SetSceneEditColors();
		scene->OnStop();
	}

	void EditorLayer::ClearScene()
	{
		glm::ivec2 viewportSize = scene->GetViewportSize();
		scene = CreateRef<Scene>();
		scene->OnViewportResize(viewportSize.x, viewportSize.y);
		selectedEntity = {};
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
			currentFilepath.clear();
			ClearScene();
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
			auto filepath = FileDialog::OpenFile("GBC Scene (*.gscn)\0*.gscn\0");
			if (filepath)
				OpenSceneFile(*filepath);
		}
	}

	void EditorLayer::OpenSceneFile(const std::string& filepath)
	{
		currentFilepath = filepath;
		ClearScene();
		SceneSerializer serializer(scene);
		serializer.Deserialize(currentFilepath);
	}

	void EditorLayer::SaveScene()
	{
		if (!currentFilepath.empty())
		{
			hasUnsavedChanges = false;

			SceneSerializer serializer(scene);
			serializer.Serialize(currentFilepath);
		}
		else
			SaveAsScene();
	}

	void EditorLayer::SaveAsScene()
	{
		auto filepath = FileDialog::SaveFile("GBC Scene (*.gscn)\0*.gscn\0");
		if (filepath)
		{
			// Add extension to extensionless path
			if (!filepath->ends_with(".gscn"))
				*filepath += ".gscn";

			currentFilepath = *filepath;
			SaveScene();
		}
	}
}
