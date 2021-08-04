#include "ScenePropertiesPanel.h"
#include "imgui/imgui.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/MeshComponent.h"
#include "GBC/Scene/Components/RenderableComponent.h"
#include "GBC/IO/FileDialog.h"

namespace gbc
{
	template<typename T, typename Func>
	static void DrawComponent(const std::string& label, int columnCount, Entity entity, bool removable, Func func)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::PushID(label.c_str());

			ImFont* font = ImGui::GetFont();
			ImGuiStyle& style = ImGui::GetStyle();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

			float lineHeight = ImGui::GetFrameHeight();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.c_str());

			// TODO: This will eventually have more than just remove component, i.e. other settings.
			ImGui::SameLine(contentRegionAvailable.x - lineHeight + style.FramePadding.x);
			if (ImGui::Button("+", {lineHeight, lineHeight}))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (removable && ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				ImGui::Unindent();
				ImGuiHelper::BeginTable(label.c_str(), columnCount);
				func(entity.GetComponent<T>());
				ImGuiHelper::EndTable();
				ImGui::Indent();
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();

			ImGui::PopID();
		}
	}

	template<typename T>
	static bool DrawAddComponent(const std::string& label, Entity entity)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(label.c_str()))
			{
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
			else
				return false;
		}
		return true;
	}

	ScenePropertiesPanel::ScenePropertiesPanel(const std::string& name, Entity& selectedEntity)
		: Panel(name), selectedEntity(selectedEntity) {}

	void ScenePropertiesPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);
			Update();

			if (selectedEntity)
			{
				// TODO: ImGui::PushID((void*)selectedEntity.GetUUID());

				// This doesn't work and I have no idea why!??!?!???
				ImGui::PushID((void*)static_cast<uint64_t>(static_cast<uint32_t>(selectedEntity)));

				{
					ImGui::PushID("TagComponent");
					ImGuiHelper::TextEdit(&selectedEntity.GetComponent<TagComponent>().tag);
					ImGui::PopID();
				}

				ImGui::Separator();

				DrawComponent<TransformComponent>("Transform", 2, selectedEntity, false, [](TransformComponent& component)
				{
					ImGuiHelper::FloatEdit3("Translation", &component.translation.x);
					ImGuiHelper::NextTableColumn();
					glm::vec3 rotation = glm::degrees(component.rotation);
					if (ImGuiHelper::FloatEdit3("Rotation", &rotation.x, 0.0f, 1.0f))
						component.rotation = glm::radians(rotation);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit3("Scale", &component.scale.x, 1.0f);
				});

				DrawComponent<CameraComponent>("Camera", 2, selectedEntity, true, [](CameraComponent& component)
				{
					SceneCamera& camera = component.camera;

					ImGuiHelper::Checkbox("Primary", &component.primary);
					ImGuiHelper::NextTableColumn();

					static constexpr const char* names[] {"Perspective", "Orthographic"};
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

				DrawComponent<MeshComponent>("Mesh", 2, selectedEntity, true, [](MeshComponent& component)
				{
					const char* buttonText = "Null";
					if (!component.filepath.empty())
						buttonText = component.filepath.c_str();

					ImGuiHelper::ButtonDragDropTarget("Model", buttonText, "CONTENT_BROWSER_ITEM", [&component](const ImGuiPayload* payload)
					{
						std::string filepath = static_cast<const char*>(payload->Data);

						if (filepath.ends_with(".obj"))
						{
							OBJModel model;
							if (OBJLoader::LoadOBJ(filepath, model))
							{
								component.mesh = CreateRef<BasicMesh>(std::move(model));
								component.filepath = std::move(filepath);
							}
						}
					});
				});

				DrawComponent<RenderableComponent>("Renderable", 2, selectedEntity, true, [](RenderableComponent& component)
				{
					ImGuiHelper::ColorEdit4("Tint Color", &component.color.x);
					ImGuiHelper::NextTableColumn();

					const char* buttonText = "Null";
					if (component.texture && component.texture->GetTexture())
						buttonText = component.texture->GetTexture()->GetFilepath().c_str();

					ImGuiHelper::ButtonDragDropTarget("Texture", buttonText, "CONTENT_BROWSER_ITEM", [&component](const ImGuiPayload* payload)
					{
						std::string filepath = (const char*)(payload->Data);
						auto localTexture = CreateRef<LocalTexture2D>(filepath);
						if (localTexture)
							component.texture = Texture::CreateRef(localTexture);
					});
					ImGuiHelper::NextTableColumn();

					ImGuiHelper::FloatEdit("Tiling Factor", &component.tilingFactor);
				});

				if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight())))
					ImGui::OpenPopup("AddComponent");
				if (ImGui::BeginPopup("AddComponent"))
				{
					bool allComponentsAdded = true;
					allComponentsAdded &= DrawAddComponent<CameraComponent>("Camera", selectedEntity);
					allComponentsAdded &= DrawAddComponent<MeshComponent>("Mesh", selectedEntity);
					allComponentsAdded &= DrawAddComponent<RenderableComponent>("Renderable", selectedEntity);
					allComponentsAdded &= DrawAddComponent<TransformComponent>("Transform", selectedEntity);
					if (allComponentsAdded)
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}

			ImGui::End();
		}
	}
}
