#include "ScenePropertiesPanel.h"
#include "imgui/imgui.h"
#include "GBC/ImGui/ImGuiHelper.h"
#include "GBC/Scene/Components/TagComponent.h"
#include "GBC/Scene/Components/TransformComponent.h"
#include "GBC/Scene/Components/CameraComponent.h"
#include "GBC/Scene/Components/RenderableComponent.h"

namespace gbc
{
	template<typename T, typename Func>
	static void DrawComponent(const std::string& label, Entity entity, bool removable, Func func)
	{
		if (entity.Has<T>())
		{
			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			T& component = entity.Get<T>();

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 4.0f));

			ImFont* font = ImGui::GetFont();
			ImGuiStyle& style = ImGui::GetStyle();
			float lineHeight = font->FontSize + style.FramePadding.y * 2.0f;

			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label.c_str());
			ImGui::PopStyleVar();

			// TODO: this position is likely not right with other fonts
			ImGui::SameLine(contentRegionAvailable.x - font->FontSize * 0.5f - style.FramePadding.y + 1.0f);
			if (ImGui::Button("+", {lineHeight, lineHeight}))
				ImGui::OpenPopup("RemoveComponent");

			bool removeComponent = false;
			if (removable && ImGui::BeginPopup("RemoveComponent"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				func(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.Remove<T>();
		}
	}

	template<typename T>
	static void DrawAddComponent(const std::string& label, Entity entity)
	{
		if (!entity.Has<T>() && ImGui::MenuItem(label.c_str()))
		{
			entity.Add<T>();
			ImGui::CloseCurrentPopup();
		}
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
					std::string& tag = selectedEntity.Get<TagComponent>().tag;

					char buffer[256]{0};
					strcpy_s(buffer, sizeof(buffer), tag.c_str());

					if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
						tag = buffer;
				}

				DrawComponent<TransformComponent>("Transform", selectedEntity, false, [](TransformComponent& component)
				{
					ImGuiHelper::DrawVec3Control("Translation", component.translation);
					ImGuiHelper::DrawVec3Control("Rotation", component.rotation);
					ImGuiHelper::DrawVec3Control("Scale", component.scale, 1.0f);
				});

				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("AddComponent");
				if (ImGui::BeginPopup("AddComponent"))
				{
					DrawAddComponent<TransformComponent>("Transform", selectedEntity);
					DrawAddComponent<CameraComponent>("Camera", selectedEntity);
					DrawAddComponent<RenderableComponent>("Renderable", selectedEntity);
					ImGui::EndPopup();
				}

				DrawComponent<RenderableComponent>("Renderable", selectedEntity, true, [](RenderableComponent& component)
				{
					constexpr const char* items[]{"None", "Texture", "Framebuffer"};
					const char* name = "None";
					int currentItem = 0;
					switch (component.renderable.GetType())
					{
						case BasicRenderable::Type::Texture: name = "Texture"; currentItem = 1; break;
						case BasicRenderable::Type::Framebuffer: name = "Framebuffer"; currentItem = 2; break;
					}

					int selectedItem = currentItem;
					if (ImGui::BeginCombo("Type", name))
					{
						for (int i = 0; i < 3; i++)
						{
							bool selected = selectedItem == i;
							if (ImGui::Selectable(items[i], selected))
								selectedItem = i;
							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					if (currentItem != selectedItem)
					{
						switch (selectedItem)
						{
							case 0: component.renderable.SetType(BasicRenderable::Type::None); break;
							case 1: component.renderable.SetType(BasicRenderable::Type::Texture); break;
							case 2: component.renderable.SetType(BasicRenderable::Type::Framebuffer); break;
						}
					}

					// TODO: actually implement this
					switch (component.renderable.GetType())
					{
						case BasicRenderable::Type::Texture:
							break;
						case BasicRenderable::Type::Framebuffer:
							break;
					}
				});
			}

			ImGui::End();
		}
	}
}
