#include "SceneHierarchyPanel.h"
#include <imgui/imgui.h>
#include "GBC/Core/Input.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"

namespace gbc
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::string& name, Ref<Scene>& context, Entity& selectedEntity)
		: Panel(name), context(context), selectedEntity(selectedEntity) {}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			// Fake child for drag/drop
			// TODO: add reordering of elements
			//	add a single blue line at the index to move the entity to
			//		with the proper length depending on child status
			ImGui::BeginChild("SceneHierarchyPanelChild");
			Update();

			for (auto entity : context->entities)
				DrawEntityNode({ entity, context.get() });
			if (entityToDelete)
			{
				if (selectedEntity && (selectedEntity == entityToDelete || selectedEntity.IsSubChildOf(entityToDelete)))
					selectedEntity = {};
				context->DestroyEntity(entityToDelete);
				entityToDelete = {};
			}

			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
				selectedEntity = {};

			if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Empty Entity"))
						selectedEntity = context->CreateEntity("Empty Entity");
					if (ImGui::MenuItem("Camera"))
					{
						selectedEntity = context->CreateEntity("Camera");
						selectedEntity.Add<CameraComponent>();
					}
					if (ImGui::MenuItem("Quad"))
					{
						selectedEntity = context->CreateEntity("Quad");
						selectedEntity.Add<SpriteRendererComponent>();
					}
					if (ImGui::BeginMenu("Physics"))
					{
						if (ImGui::MenuItem("Static"))
						{
							selectedEntity = context->CreateEntity("Static Physics Entity");
							selectedEntity.Add<SpriteRendererComponent>();
							selectedEntity.Add<BoxCollider2DComponent>();
							selectedEntity.Add<Rigidbody2DComponent>(Rigidbody2DComponent::BodyType::Static);
						}
						if (ImGui::MenuItem("Dynamic"))
						{
							selectedEntity = context->CreateEntity("Dynamic Physics Entity");
							selectedEntity.Add<SpriteRendererComponent>();
							selectedEntity.Add<BoxCollider2DComponent>();
							selectedEntity.Add<Rigidbody2DComponent>();
						}
						if (ImGui::MenuItem("Kinematic"))
						{
							selectedEntity = context->CreateEntity("Kinematic Physics Entity");
							selectedEntity.Add<SpriteRendererComponent>();
							selectedEntity.Add<BoxCollider2DComponent>();
							selectedEntity.Add<Rigidbody2DComponent>(Rigidbody2DComponent::BodyType::Kinematic);
						}
						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}

			ImGui::EndChild();
			MakeCurrentItemDragDropTarget({});
			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (selectedEntity == entity)
			flags |= ImGuiTreeNodeFlags_Selected;
		if (!entity.HasChildren())
			flags |= ImGuiTreeNodeFlags_Leaf;
		auto id = (void*)static_cast<size_t>(static_cast<uint32_t>(entity));
		bool opened = ImGui::TreeNodeEx(id, flags, "%s", entity.Get<TagComponent>().tag.c_str());

		if (ImGui::BeginPopupContextItem(nullptr, ImGuiMouseButton_Right))
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityToDelete = entity;
			// TODO: should I set selectedEntity to the duplicated entity?
			if (ImGui::MenuItem("Duplicate Entity"))
				context->DuplicateEntity(entity);
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
			selectedEntity = entity;

		MakeCurrentItemDragDropSource(entity);
		MakeCurrentItemDragDropTarget(entity);

		if (opened)
		{
			for (Entity child = entity.GetFirstChild(); child; child = child.GetNextSibling())
				DrawEntityNode(child);
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::MakeCurrentItemDragDropSource(Entity source)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::Text(source.GetName().c_str());

			ImGui::SetDragDropPayload("SCENE_HIERARCHY_ITEM", &source, sizeof(source), ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
	}

	void SceneHierarchyPanel::MakeCurrentItemDragDropTarget(Entity target)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGuiHelper::AcceptDragDropPayload("SCENE_HIERARCHY_ITEM",
				[target](void* payloadData)
				{
					if (!target)
						return true;
					Entity source = *static_cast<Entity*>(payloadData);
					return !target.IsSubChildOf(source);
				}))
			{
				Entity child = *static_cast<Entity*>(payload->Data);
				child.SetParent(target);
			}

			ImGui::EndDragDropTarget();
		}
	}
}
