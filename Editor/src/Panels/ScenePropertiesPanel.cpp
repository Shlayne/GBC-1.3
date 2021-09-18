#include "ScenePropertiesPanel.h"
#include <imgui/imgui.h>
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/IO/FileDialog.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/SpriteRendererComponent.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/Physics/BoxCollider2DComponent.h"
#include "GBC/Scene/Components/Physics/Rigidbody2DComponent.h"

namespace gbc
{
	template<typename T, typename Func>
	static void DrawComponent(const std::string& label, int columnCount, Entity entity, bool removable, Func func)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::PushID(label.c_str());
			ImGui::PushID(static_cast<uint32_t>(entity));

			ImFont* font = ImGui::GetFont();
			ImGuiStyle& style = ImGui::GetStyle();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth;

			float lineHeight = ImGui::GetFrameHeight();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			void* id = (void*)(((size_t)(void*)label.c_str()) ^ typeid(T).hash_code());
			bool open = ImGui::TreeNodeEx(id, flags, label.c_str());

			// TODO: This will eventually have more than just remove component, i.e. other settings.
			ImGui::SameLine(contentRegionAvailable.x - lineHeight + style.FramePadding.x);
			if (ImGui::Button("+", { lineHeight, lineHeight }))
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
					if (ImGuiHelper::FloatEdit3("Rotation", &rotation.x))
						component.rotation = glm::radians(rotation);
					ImGuiHelper::NextTableColumn();
					ImGuiHelper::FloatEdit3("Scale", &component.scale.x);
				});

				DrawComponent<CameraComponent>("Camera", 2, selectedEntity, true, [](CameraComponent& component)
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

				DrawComponent<SpriteRendererComponent>("Sprite Renderer", 2, selectedEntity, true, [](SpriteRendererComponent& component)
				{
					ImGuiHelper::ColorEdit4("Tint Color", &component.color.x);
					ImGuiHelper::NextTableColumn();

					std::string buttonText = "Null";
					if (component.texture && component.texture->GetTexture())
						buttonText = component.texture->GetTexture()->GetFilepath().string();

					ImGuiHelper::ButtonDragDropTarget("Texture", buttonText.c_str(), "CONTENT_BROWSER_ITEM", [&component](const ImGuiPayload* payload)
					{
						std::filesystem::path filepath = (const wchar_t*)(payload->Data);
						if (filepath.native().ends_with(L".png")) // TODO: support other file types
						{
							auto localTexture = LocalTexture2D::Create(filepath);
							if (localTexture)
							{
								TextureSpecification specs = component.texture ? component.texture->GetSpecification() : TextureSpecification{};
								specs.texture = localTexture;
								component.texture = Texture2D::Create(specs);
							}
						}
					});
					ImGuiHelper::NextTableColumn();

					ImGuiHelper::FloatEdit2("Tiling Factor", &component.tilingFactor.x);
					ImGuiHelper::NextTableColumn();

					// TODO: all of what's in this if statement should be moved to another panel similar to unity
					// because there are going to be way more than just these things in a texture
					if (component.texture)
					{
						TextureSpecification specs = component.texture->GetSpecification();
						bool changed = false;

						static constexpr const char* names1[]{ "Linear", "Nearest" };
						static constexpr const char* names2[]{ "ClampToEdge", "Repeat" };

						int selectedItem = static_cast<int>(specs.minFilter);
						if (changed = ImGuiHelper::Combo("Min Filter", &selectedItem, names1, sizeof(names1) / sizeof(*names1)))
						{
							specs.minFilter = static_cast<TextureFilterMode>(selectedItem);
							changed = true;
						}
						ImGuiHelper::NextTableColumn();
						selectedItem = static_cast<int>(specs.magFilter);
						if (ImGuiHelper::Combo("Mag Filter", &selectedItem, names1, sizeof(names1) / sizeof(*names1)) && !changed)
						{
							specs.magFilter = static_cast<TextureFilterMode>(selectedItem);
							changed = true;
						}
						ImGuiHelper::NextTableColumn();
						selectedItem = static_cast<int>(specs.wrapS);
						if (ImGuiHelper::Combo("Wrap S", &selectedItem, names2, sizeof(names2) / sizeof(*names2)) && !changed)
						{
							specs.wrapS = static_cast<TextureWrapMode>(selectedItem);
							changed = true;
						}
						ImGuiHelper::NextTableColumn();
						selectedItem = static_cast<int>(specs.wrapT);
						if (ImGuiHelper::Combo("Wrap T", &selectedItem, names2, sizeof(names2) / sizeof(*names2)) && !changed)
						{
							specs.wrapT = static_cast<TextureWrapMode>(selectedItem);
							changed = true;
						}

						if (changed)
							component.texture = Texture2D::Create(specs);
					}
				});

				DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", 2, selectedEntity, true, [](Rigidbody2DComponent& component)
				{
					static constexpr const char* names[]{ "Static", "Dynamic", "Kinematic" };

					int selectedItem = static_cast<int>(component.bodyType);
					if (ImGuiHelper::Combo("Body Type", &selectedItem, names, sizeof(names) / sizeof(*names)))
						component.bodyType = static_cast<Rigidbody2DComponent::BodyType>(selectedItem);
					ImGuiHelper::NextTableColumn();

					ImGuiHelper::Checkbox("Fixed Rotation", &component.fixedRotation);
				});

				DrawComponent<BoxCollider2DComponent>("Box Collider", 2, selectedEntity, true, [](BoxCollider2DComponent& component)
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

				ImGui::Separator();

				static constexpr char addComponentText[] = "Add Component";
				ImGuiStyle& style = ImGui::GetStyle();
				ImVec2 addComponentButtonSize = ImGui::CalcTextSize(addComponentText);
				addComponentButtonSize.x += style.FramePadding.x * 2.0f;
				addComponentButtonSize.y += style.FramePadding.y * 2.0f;

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowContentRegionWidth() - addComponentButtonSize.x) * 0.5f);
				if (ImGui::Button(addComponentText, addComponentButtonSize))
					ImGui::OpenPopup("AddComponent");
				if (ImGui::BeginPopup("AddComponent"))
				{
					bool allComponentsAdded = true;
					allComponentsAdded &= DrawAddComponent<CameraComponent>("Camera", selectedEntity);
					allComponentsAdded &= DrawAddComponent<SpriteRendererComponent>("Sprite Renderer", selectedEntity);
					allComponentsAdded &= DrawAddComponent<TransformComponent>("Transform", selectedEntity);
					ImGui::Separator(); // New section: physics
					allComponentsAdded &= DrawAddComponent<BoxCollider2DComponent>("Box Collider", selectedEntity);
					allComponentsAdded &= DrawAddComponent<Rigidbody2DComponent>("Rigidbody 2D", selectedEntity);
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
