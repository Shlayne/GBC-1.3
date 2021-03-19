#include "ScenePropertiesPanel.h"
#include "imgui/imgui.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/MeshComponent.h"
#include "GBC/Scene/Components/RenderableComponent.h"

namespace gbc
{
	template<typename T, typename Func>
	static void DrawComponent(const std::string& label, Entity entity, bool removable, Func func)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::PushID(label.c_str());

			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImFont* font = ImGui::GetFont();
			ImGuiStyle& style = ImGui::GetStyle();
			float lineHeight = font->FontSize + style.FramePadding.y * 2.0f;

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.c_str());

			// TODO: This will eventually have more than just remove component, i.e. other settings.
			ImGui::SameLine(contentRegionAvailable.x - lineHeight + style.FramePadding.x * 1.5f);
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
				func(entity.GetComponent<T>());
				ImGui::Separator();
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
		if (!entity.HasComponent<T>() && ImGui::MenuItem(label.c_str()))
		{
			entity.AddComponent<T>();
			ImGui::CloseCurrentPopup();
			return true;
		}
		return false;
	}

	ScenePropertiesPanel::ScenePropertiesPanel(const std::string& name, Entity& selectedEntity)
		: Panel(name), selectedEntity(selectedEntity) {}

	void ScenePropertiesPanel::OnImGuiRender()
	{
		if (enabled)
		{
			ImGui::Begin(name.c_str(), &enabled);

			if (selectedEntity)
			{
				//if (selectedEntity.Has<TagComponent>())
				{
					ImGui::PushID("TagComponent");

					std::string& tag = selectedEntity.GetComponent<TagComponent>().tag;

					static constexpr size_t bufferSize = 256;
					char buffer[bufferSize]{0};
					GBC_CORE_ASSERT(tag.size() < bufferSize);
					strcpy_s(buffer, sizeof(buffer), tag.c_str());

					if (ImGui::InputText("", buffer, sizeof(buffer)))
						tag = buffer;

					ImGui::PopID();
				}

				ImGui::SameLine();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("AddComponent");
				if (ImGui::BeginPopup("AddComponent"))
				{
					if (!DrawAddComponent<TransformComponent>("Transform", selectedEntity))
					if (!DrawAddComponent<CameraComponent>("Camera", selectedEntity))
					if (!DrawAddComponent<MeshComponent>("Mesh", selectedEntity))
					DrawAddComponent<RenderableComponent>("Renderable", selectedEntity);
					ImGui::EndPopup();
				}
				ImGui::PopItemWidth();

				DrawComponent<TransformComponent>("Transform", selectedEntity, true, [](TransformComponent& component)
				{
					ImGuiHelper::Float3Edit("Translation", &component.translation.x);
					glm::vec3 rotation = glm::degrees(component.rotation);
					if (ImGuiHelper::Float3Edit("Rotation", &rotation.x, 0.0f, 1.0f))
						component.rotation = glm::radians(rotation);
					ImGuiHelper::Float3Edit("Scale", &component.scale.x, 1.0f);
				});

				DrawComponent<CameraComponent>("Camera", selectedEntity, true, [](CameraComponent& component)
				{
					SceneCamera& camera = component.camera;

					ImGuiHelper::Checkbox("Primary", &component.primary);

					static constexpr const char* names[]{"Perspective", "Orthographic"};
					int selectedItem = static_cast<int>(camera.GetProjectionType());
					ImGuiHelper::Combo("Projection", &selectedItem, names, sizeof(names) / sizeof(const char*));

					switch (selectedItem)
					{
						case 0:
						{
							component.camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);

							float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
							if (ImGuiHelper::FloatEdit("Size", &perspectiveFOV), 1.0f)
								camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));

							float perspectiveNearClip = camera.GetPerspectiveNearClip();
							if (ImGuiHelper::FloatEdit("Near Clip", &perspectiveNearClip, 0.1f))
								camera.SetPerspectiveNearClip(perspectiveNearClip);

							float perspectiveFarClip = camera.GetPerspectiveFarClip();
							if (ImGuiHelper::FloatEdit("Far Clip", &perspectiveFarClip, 0.1f))
								camera.SetPerspectiveFarClip(perspectiveFarClip);
							break;
						}
						case 1:
						{
							component.camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

							ImGuiHelper::Checkbox("Fixed Aspect", &component.fixedAspectRatio);

							float orthographicSize = camera.GetOrthographicSize();
							if (ImGuiHelper::FloatEdit("Size", &orthographicSize, 0.1f))
								camera.SetOrthographicSize(orthographicSize);

							float orthographicNearClip = camera.GetOrthographicNearClip();
							if (ImGuiHelper::FloatEdit("Near Clip", &orthographicNearClip, 0.1f))
								camera.SetOrthographicNearClip(orthographicNearClip);

							float orthographicFarClip = camera.GetOrthographicFarClip();
							if (ImGuiHelper::FloatEdit("Far Clip", &orthographicFarClip, 0.1f))
								camera.SetOrthographicFarClip(orthographicFarClip);
							break;
						}
					}
				});

				DrawComponent<MeshComponent>("Mesh", selectedEntity, true, [](MeshComponent& component)
				{
					ImGuiHelper::BeginColumns("Mesh");
					ImGui::Text("%s", component.mesh != nullptr ? component.mesh->filepath.c_str() : "Unknown");
					ImGuiHelper::EndColumns();
				});

				DrawComponent<RenderableComponent>("Renderable", selectedEntity, true, [](RenderableComponent& component)
				{
					ImGuiHelper::BeginColumns("Texture");
					ImGui::Text("%s", component.texture != nullptr ? component.texture->GetTexture()->GetFilePath().c_str() : "Unknown");
					ImGuiHelper::EndColumns();
					ImGuiHelper::ColorEdit4("Tint Color", &component.tintColor.x);
				});
			}

			ImGui::End();
		}
	}
}
