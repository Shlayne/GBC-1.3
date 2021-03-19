#include "SceneViewportPanel.h"
#include "imgui/imgui.h"
#include "imguizmo/ImGuizmo.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "glm/gtc/type_ptr.hpp" // TODO: Editor precompiled header
#include "GBC/Math/Math.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "GBC/Core/Application.h"
#include "GBC/Core/input.h"

namespace gbc
{
	SceneViewportPanel::SceneViewportPanel(const std::string& name, bool& viewportSizeChanged, bool& viewportFocused, bool& viewportHovered, glm::ivec2& viewportSize, glm::vec2& viewportPos, glm::vec2& absoluteMousePos, Ref<Framebuffer>& framebuffer, Ref<Scene>& context, Entity& selectedEntity, int& gizmoType, EditorCamera& editorCamera)
		: Panel(name), viewportSizeChanged(viewportSizeChanged), viewportFocused(viewportFocused), viewportHovered(viewportHovered), viewportSize(viewportSize), viewportPos(viewportPos), absoluteMousePos(absoluteMousePos), framebuffer(framebuffer), context(context), selectedEntity(selectedEntity), gizmoType(gizmoType), editorCamera(editorCamera) {}

	void SceneViewportPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
			ImGui::Begin(name.c_str(), &enabled);
			ImGui::PopStyleVar();

			viewportFocused = ImGui::IsWindowFocused();
			viewportHovered = ImGui::IsWindowHovered();
			ImVec2 viewportOffset = ImGui::GetCursorPos();

			ImVec2 size = ImGui::GetContentRegionAvail();
			viewportSizeChanged = size.x != viewportSize.x || size.y != viewportSize.y;
			viewportSize = {size.x, size.y};
			ImGui::Image((void*)(size_t)framebuffer->GetColorAttachment(), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

			ImVec2 windowPos = ImGui::GetWindowPos();
			viewportPos = {windowPos.x + viewportOffset.x,	windowPos.y + viewportOffset.y};
			ImVec2 mousePos = ImGui::GetMousePos();
			absoluteMousePos = {mousePos.x, mousePos.y};

			// Gizmos
			if (selectedEntity && gizmoType != -1)
			{
				// TODO: orthographic projections
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				// Get window info
				ImVec2 windowPosition = ImGui::GetWindowPos();
				ImVec2 windowSize = ImGui::GetWindowSize();
				ImGuizmo::SetRect(windowPosition.x, windowPosition.y, windowSize.x, windowSize.y);

				// Get camera info
				//Entity cameraEntity = context->GetPrimaryCameraEntity();
				//const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
				//const glm::mat4& projection = camera.GetProjection();
				//glm::mat4 view = glm::inverse((glm::mat4)cameraEntity.GetComponent<TransformComponent>());

				// Get editor camera info
				glm::mat4 view = editorCamera.GetView();
				glm::mat4 projection = editorCamera.GetProjection();
				
				// Get entity info
				TransformComponent& transformComponent = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = transformComponent;

				// Get snapping info
				bool snap = Input::IsKeyPressed(Keycode::LeftControl);
				float snapValue = 0.5f;
				if (gizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;
				float snapValues[3]{snapValue, snapValue, snapValue};

				// Draw gizmos
				if (ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), (ImGuizmo::OPERATION)gizmoType,
					ImGuizmo::MODE::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr))
				{
					glm::vec3 translation, rotation, scale;
					if (Math::Decompose(transform, translation, rotation, scale))
					{
						transformComponent.translation = translation;
						transformComponent.rotation = rotation;
						transformComponent.scale = scale;
					}
				}
			}

			ImGui::End();
		}
	}
}
