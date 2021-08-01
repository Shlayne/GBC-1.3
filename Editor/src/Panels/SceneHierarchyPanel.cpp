#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/MeshComponent.h"
#include "GBC/Scene/Components/TagComponent.h"

namespace gbc
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::string& name, Ref<Scene>& context, Entity& selectedEntity)
		: Panel(name), context(context), selectedEntity(selectedEntity) {}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, ImGui::GetStyle().WindowPadding.y));
			ImGui::Begin(name.c_str(), &enabled);
			ImGui::PopStyleVar();
			Update();

			// TODO: entt iterates over its entities in a reverse order, so unreverse it.
			context->registry.each([&](entt::entity handle)
			{
				Entity entity(handle, context.get());
				DrawEntityNode(entity);
			});

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				selectedEntity = {};

			if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Empty Entity"))
						context->CreateEntity("Empty Entity");
					if (ImGui::MenuItem("Camera"))
					{
						Entity entity = context->CreateEntity("Camera");
						entity.AddComponent<CameraComponent>();
					}
					if (ImGui::BeginMenu("Mesh"))
					{
						if (ImGui::MenuItem("Empty Mesh"))
						{
							Entity entity = context->CreateEntity("Empty Mesh");
							entity.AddComponent<MeshComponent>();
						}
						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (selectedEntity == entity)
			flags |= ImGuiTreeNodeFlags_Selected;
		auto id = (void*)static_cast<size_t>(static_cast<uint32_t>(entity));
		bool opened = ImGui::TreeNodeEx(id, flags, "%s", entity.GetComponent<TagComponent>().tag.c_str());

		bool removeEntity = false;
		if (ImGui::BeginPopupContextItem(nullptr, ImGuiMouseButton_Right))
		{
			if (ImGui::MenuItem("Delete Entity"))
				removeEntity = true;
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
			selectedEntity = entity;

		if (opened)
			ImGui::TreePop();

		if (removeEntity)
		{
			if (selectedEntity == entity)
				selectedEntity = {};
			context->DestroyEntity(entity);
		}
	}
}
