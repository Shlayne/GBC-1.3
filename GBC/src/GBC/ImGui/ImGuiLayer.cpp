#include "gbcpch.h"
#if GBC_ENABLE_IMGUI
#include "ImGuiLayer.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imguizmo/ImGuizmo.h"
#include "glfw/glfw3.h"
#include "GBC/Core/Application.h"

namespace gbc
{
	ImGuiLayer::ImGuiLayer()
	{
		GBC_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiHelper::Init();
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 460"); // TODO: this version should be gotten from the window
	}

	ImGuiLayer::~ImGuiLayer()
	{
		GBC_PROFILE_FUNCTION();

		ImGuiHelper::Shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		GBC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		GBC_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Window& window = Application::Get().GetWindow();
		io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(context);
		}
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.handled |= event.IsInCategory(EventCategory_Keyboard) && io.WantCaptureKeyboard ||
							 event.IsInCategory(EventCategory_Mouse) && io.WantCaptureMouse;
		}
	}
}
#endif
