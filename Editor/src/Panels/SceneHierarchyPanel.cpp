#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "GBC/Scene/Components/TagComponent.h"

namespace gbc
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::string& name, const Ref<Scene>& context)
		: Panel(name)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		this->context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, ImGui::GetStyle().WindowPadding.y));
			ImGui::Begin(name.c_str(), &enabled);
			ImGui::PopStyleVar();

			context->registry.each([&](entt::entity handle)
			{
				Entity entity(handle, context.get());
				DrawEntityNode(entity);
			});

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				selectedEntity = {};

			if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					context->CreateEntity("Empty Entity");
				ImGui::EndPopup();
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
		if (selectedEntity == entity)
			flags |= ImGuiTreeNodeFlags_Selected;
		bool opened = ImGui::TreeNodeEx((void*)(size_t)(uint32_t)entity, flags, "%s", entity.Get<TagComponent>().tag.c_str());

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
			context->RemoveEntity(entity);
			selectedEntity = {};
		}
	}
}
