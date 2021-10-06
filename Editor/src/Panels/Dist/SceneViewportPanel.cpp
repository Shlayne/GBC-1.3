#include "SceneViewportPanel.h"
#include "GBC/Core/Application.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/Core/Input.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Math/Math.h"
#include "GBC/Rendering/EditorCamera.h"
#include "GBC/Rendering/Framebuffer.h"
#include "GBC/Scene/Entity.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include <functional>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include "Layers/EditorLayer.h"

namespace gbc
{
	SceneViewportPanel::SceneViewportPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void SceneViewportPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f , 0.0f });
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
			ImGui::Begin(name.c_str(), &enabled);
			ImGui::PopStyleVar(2);
			Update();

			auto& framebuffer = editorLayer->framebuffer;
			auto& selectedEntity = editorLayer->selectedEntity;
			auto& gizmoType = editorLayer->gizmoType;
			auto& canRenderGizmos = editorLayer->canRenderGizmos;
			auto& editorCamera = editorLayer->editorCamera;
			auto& canUseGizmos = editorLayer->canUseGizmos;

			auto textureID = (void*)static_cast<size_t>(framebuffer->GetColorAttachment());
			ImVec2 textureSize{ static_cast<float>(size.x), static_cast<float>(size.y) };
			ImGui::Image(textureID, textureSize, { 0.0f, 1.0f }, { 1.0f, 0.0f });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGuiHelper::AcceptDragDropPayload("CONTENT_BROWSER_ITEM",
					[](void* payloadData) { return IsSceneFile(static_cast<const wchar_t*>(payloadData)); }))
				{
					// TODO: calling this openScene method can caused unsaved work to be lost!
					editorLayer->OpenSceneFile(static_cast<const wchar_t*>(payload->Data));
				}
				ImGui::EndDragDropTarget();
			}

			// Gizmos
			if (selectedEntity && gizmoType != ImGuizmo::OPERATION::NONE && canRenderGizmos)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				// Get window info
				ImGuizmo::SetRect(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(size.x), static_cast<float>(size.y));

				// Get editor camera info
				const glm::mat4& view = editorCamera.GetView();
				const glm::mat4& projection = editorCamera.GetProjection();
				
				// Get entity info
				auto& transformComponent = selectedEntity.Get<TransformComponent>();
				glm::mat4 transform = transformComponent;

				// Get snapping info
				bool snap = Input::IsKeyPressed(Keycode::LeftControl);
				float snapValue = 0.5f;
				if (gizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;
				float snapValues[3]{ snapValue, snapValue, snapValue };

				// Draw gizmos
				if (ImGuizmo::Manipulate(&view[0].x, &projection[0].x, static_cast<ImGuizmo::OPERATION>(gizmoType),
					ImGuizmo::MODE::LOCAL, &transform[0].x, nullptr, snap ? snapValues : nullptr) && canUseGizmos)
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
