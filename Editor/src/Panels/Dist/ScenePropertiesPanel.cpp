#include "ScenePropertiesPanel.h"
#include "GBC/Core/Application.h"
#include "GBC/Core/FileTypes.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/IO/FileDialog.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/Mesh3DComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"
#include <imgui/imgui.h>
#include "Layers/EditorLayer.h"

namespace gbc
{
	template<typename Component, typename Func>
	static void DrawComponent(const std::string& label, Entity entity, bool removable, Func func)
	{
		if (entity.Has<Component>())
		{
			ImGui::PushID(static_cast<uint32_t>(entity));
			ImGui::PushID(label.c_str());

			ImFont* font = ImGui::GetFont();
			ImGuiStyle& style = ImGui::GetStyle();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

			float lineHeight = ImGui::GetFrameHeight();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			void* id = (void*)(((size_t)(void*)label.c_str()) ^ typeid(Component).hash_code());
			bool open = ImGui::TreeNodeEx(id, flags, label.c_str());

			// TODO: This will eventually have more than just remove component, i.e. other settings.
			ImGui::SameLine(contentRegionAvailable.x - lineHeight + style.FramePadding.x);
			if (ImGui::Button("+", ImVec2{ lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (removable && ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				ImGui::Unindent();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.0f);
				if (ImGuiHelper::BeginTable(label.c_str(), 2))
				{
					func(entity.Get<Component>());
					ImGuiHelper::EndTable();
				}
				ImGui::Indent();
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.Remove<Component>();

			ImGui::PopID();
			ImGui::PopID();
		}
	}

	template<typename Component>
	static void DrawAdd(const std::string& label, Entity entity)
	{
		if (!entity.Has<Component>() && ImGui::MenuItem(label.c_str()))
		{
			entity.Add<Component>();
			ImGui::CloseCurrentPopup();
		}
	}

	static void TextureButton(Ref<Texture2D>& texture)
	{
		std::string buttonText = "Null";
		if (texture && texture->GetTexture())
			buttonText = texture->GetTexture()->GetFilepath().string();

		if (const ImGuiPayload* payload = ImGuiHelper::ButtonDragDropTarget("Texture", buttonText.c_str(), "CONTENT_BROWSER_ITEM",
			[](void* payloadData) { return IsTextureFile(static_cast<const wchar_t*>(payloadData)); }))
		{
			std::filesystem::path filepath = static_cast<const wchar_t*>(payload->Data);
			texture = Application::Get().GetAssetManager().GetOrLoadTexture(filepath);
		}
	}

	ScenePropertiesPanel::ScenePropertiesPanel(const std::string& name, EditorLayer* editorLayer)
		: Panel(name, editorLayer) {}

	void ScenePropertiesPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4.0f, 2.0f });
			ImGui::Begin(name.c_str(), &enabled);
			ImGui::PopStyleVar();
			Update();

			auto& selectedEntity = editorLayer->selectedEntity;

			if (selectedEntity)
			{
				ImGui::PushID((void*)(uint64_t)selectedEntity.GetUUID());

				{
					ImGuiStyle& style = ImGui::GetStyle();
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, style.ItemSpacing * 0.5f);

					if (ImGui::BeginTable("TagComponent", 2))
					{
						ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_None);
						ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthAuto);
						ImGui::TableNextColumn();

						auto& tag = selectedEntity.Get<TagComponent>().tag;
						ImGuiHelper::TextEdit(&tag);
						ImGui::TableNextColumn();

						std::string uuidString = std::to_string(selectedEntity.GetUUID());
						ImGui::Text(uuidString.c_str());
						ImGui::EndTable();
					}

					ImGui::PopStyleVar();
				}

				ImGui::Separator();

				DrawComponent<TransformComponent>("Transform", selectedEntity, false, [](TransformComponent& component)
				{
					ImGuiHelper::FloatEdit3("Translation", &component.translation.x);
					ImGuiHelper::NextTableColumn();
					glm::vec3 rotation = glm::degrees(component.rotation);
					if (ImGuiHelper::FloatEdit3("Rotation", &rotation.x))
						component.rotation = glm::radians(rotation);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit3("Scale", &component.scale.x);
				});

				DrawComponent<CameraComponent>("Camera", selectedEntity, true, [](CameraComponent& component)
				{
					SceneCamera& camera = component.camera;

					ImGuiHelper::Checkbox("Primary", &component.primary);
					ImGuiHelper::NextTableColumn();

					static constexpr const char* names[]{ "Perspective", "Orthographic" };
					int selectedItem = static_cast<int>(camera.GetProjectionType());
					ImGuiHelper::Combo("Projection", &selectedItem, names, sizeof(names) / sizeof(*names));
					ImGuiHelper::NextTableColumn();

					switch (selectedItem)
					{
						case 0:
						{
							component.camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);

							float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
							if (ImGuiHelper::FloatEdit("Size", &perspectiveFOV), 1.0f)
								camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));
							ImGuiHelper::NextTableColumn();

							float perspectiveNearClip = camera.GetPerspectiveNearClip();
							if (ImGuiHelper::FloatEdit("Near Clip", &perspectiveNearClip, 0.1f))
								camera.SetPerspectiveNearClip(perspectiveNearClip);
							ImGuiHelper::NextTableColumn();

							float perspectiveFarClip = camera.GetPerspectiveFarClip();
							if (ImGuiHelper::FloatEdit("Far Clip", &perspectiveFarClip, 0.1f))
								camera.SetPerspectiveFarClip(perspectiveFarClip);
							break;
						}
						case 1:
						{
							component.camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

							float orthographicSize = camera.GetOrthographicSize();
							if (ImGuiHelper::FloatEdit("Size", &orthographicSize, 0.1f))
								camera.SetOrthographicSize(orthographicSize);
							ImGuiHelper::NextTableColumn();

							float orthographicNearClip = camera.GetOrthographicNearClip();
							if (ImGuiHelper::FloatEdit("Near Clip", &orthographicNearClip, 0.1f))
								camera.SetOrthographicNearClip(orthographicNearClip);
							ImGuiHelper::NextTableColumn();

							float orthographicFarClip = camera.GetOrthographicFarClip();
							if (ImGuiHelper::FloatEdit("Far Clip", &orthographicFarClip, 0.1f))
								camera.SetOrthographicFarClip(orthographicFarClip);
							ImGuiHelper::NextTableColumn();

							ImGuiHelper::Checkbox("Fixed Aspect", &component.fixedAspectRatio);
							break;
						}
					}
				});

				DrawComponent<Mesh3DComponent>("3D Mesh", selectedEntity, true, [](Mesh3DComponent& component)
				{
					std::string buttonText = "Null";
					if (component.mesh)
						buttonText = component.mesh->filepath.string();

					if (const ImGuiPayload* payload = ImGuiHelper::ButtonDragDropTarget("Mesh", buttonText.c_str(), "CONTENT_BROWSER_ITEM",
						[](void* payloadData) { return Is3DModelFile(static_cast<const wchar_t*>(payloadData)); }))
					{
						// TODO: load obj into mesh
					}
					ImGuiHelper::NextTableColumn();

					ImGuiHelper::ColorEdit4("Tint Color", &component.tintColor.x);
					ImGuiHelper::NextTableColumn();

					TextureButton(component.texture);
				});

				DrawComponent<SpriteRendererComponent>("Sprite Renderer", selectedEntity, true, [](SpriteRendererComponent& component)
				{
					ImGuiHelper::ColorEdit4("Tint Color", &component.color.x);
					ImGuiHelper::NextTableColumn();

					ImGuiHelper::FloatEdit2("Tiling Factor", &component.tilingFactor.x);
					ImGuiHelper::NextTableColumn();

					TextureButton(component.texture);
				});

				DrawComponent<BoxCollider2DComponent>("Box Collider", selectedEntity, true, [](BoxCollider2DComponent& component)
				{
					ImGuiHelper::FloatEdit2("Size", &component.size.x, 0.1f, 0.0f, FLT_MAX);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit2("Offset", &component.offset.x);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit("Friction", &component.friction, 0.1f, 0.0f, FLT_MAX);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit("Density", &component.density, 0.1f, 0.0f, FLT_MAX);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit("Restitution", &component.restitution, 0.1f, 0.0f, FLT_MAX);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit("Restitution Threshold", &component.restitutionThreshold, 0.1f, 0.0f, FLT_MAX);
				});

				DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", selectedEntity, true, [](Rigidbody2DComponent& component)
				{
					static constexpr const char* names[]{ "Static", "Dynamic", "Kinematic" };

					int selectedItem = static_cast<int>(component.bodyType);
					if (ImGuiHelper::Combo("Body Type", &selectedItem, names, sizeof(names) / sizeof(*names)))
						component.bodyType = static_cast<Rigidbody2DComponent::BodyType>(selectedItem);
					ImGuiHelper::NextTableColumn();

					ImGuiHelper::Checkbox("Fixed Rotation", &component.fixedRotation);
				});

				ImGui::Separator();

				static constexpr char addComponentText[] = "Add Component";
				ImGuiStyle& style = ImGui::GetStyle();
				ImVec2 addComponentButtonSize = ImGui::CalcTextSize(addComponentText) + style.FramePadding * 2.0f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowContentRegionWidth() - addComponentButtonSize.x) * 0.5f);
				if (ImGui::Button(addComponentText, addComponentButtonSize))
					ImGui::OpenPopup("AddComponent");
				if (ImGui::BeginPopup("AddComponent"))
				{
					DrawAdd<CameraComponent>("Camera", selectedEntity);
					DrawAdd<Mesh3DComponent>("Mesh 3D", selectedEntity);
					DrawAdd<SpriteRendererComponent>("Sprite Renderer", selectedEntity);
					DrawAdd<BoxCollider2DComponent>("Box Collider", selectedEntity);
					DrawAdd<Rigidbody2DComponent>("Rigidbody 2D", selectedEntity);
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}

			ImGui::End();
		}
	}
}
