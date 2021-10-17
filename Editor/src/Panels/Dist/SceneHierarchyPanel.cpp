#include "SceneHierarchyPanel.h"
#include "GBC/Core/Input.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Model/MeshFactory3D.h"
#include "GBC/Scene/Components/AllComponents.h"
#include <imgui/imgui.h>
#include "Layers/EditorLayer.h"

namespace gbc
{
	SceneHierarchyPanel::SceneHierarchyPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			Update();

			auto& context = editorLayer->activeScene;
			auto& selectedEntity = editorLayer->selectedEntity;

			// Fake child for drag/drop
			// TODO: add reordering of elements
			//	add a single blue line at the index to move the entity to
			//		with the proper length depending on child status
			ImGui::BeginChild("SceneHierarchyPanelChild", { 0.0f }, false, ImGuiWindowFlags_HorizontalScrollbar);
			focused |= ImGui::IsWindowFocused();
			hovered |= ImGui::IsWindowHovered();

			for (auto entity : context->entities)
				DrawEntityNode({ entity, context.get() });
			if (entityToDelete)
			{
				if (selectedEntity && (selectedEntity == entityToDelete || selectedEntity.IsSubChildOf(entityToDelete)))
					selectedEntity = {};
				context->DestroyEntity(entityToDelete ? entityToDelete : selectedEntity);
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
					if (ImGui::MenuItem("Circle"))
					{
						selectedEntity = context->CreateEntity("Circle");
						selectedEntity.Add<CircleRendererComponent>();
					}
					if (ImGui::BeginMenu("3D Model"))
					{
						if (ImGui::MenuItem("Empty"))
						{
							selectedEntity = context->CreateEntity("Empty 3D Model");
							selectedEntity.Add<Model3DComponent>();
						}
						if (ImGui::MenuItem("Cube"))
						{
							selectedEntity = context->CreateEntity("Cube");
							selectedEntity.Add<Model3DComponent>().model = Model3D::Create(MeshFactory3D::CreateCube(), MeshFactory3D::cubeID);
						}
						if (ImGui::MenuItem("Sphere"))
						{
							selectedEntity = context->CreateEntity("Sphere");
							selectedEntity.Add<Model3DComponent>().model = Model3D::Create(MeshFactory3D::CreateSphere(), MeshFactory3D::sphereID);
						}
						if (ImGui::MenuItem("Plane"))
						{
							selectedEntity = context->CreateEntity("Plane");
							selectedEntity.Add<Model3DComponent>().model = Model3D::Create(MeshFactory3D::CreatePlane(), MeshFactory3D::planeID);
						}
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("2D Physics"))
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
		auto& context = editorLayer->activeScene;
		auto& selectedEntity = editorLayer->selectedEntity;

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
			if (ImGui::MenuItem("Duplicate Entity"))
				selectedEntity = context->DuplicateEntity(entity);
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
