#include "SandboxLayer.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

void SandboxLayer::OnAttach()
{
	GBC_PROFILE_FUNCTION();

	scene = CreateScope<Scene>();

	Entity entity = scene->CreateEntity();
	BasicMesh& mesh = entity.Add<MeshComponent>(BasicMesh(4, 6)).mesh;
	entity.Add<RenderableComponent>(Texture::CreateRef(CreateRef<LocalTexture2D>("resources/textures/grass_side.png", 4, true)));

	mesh.vertices[0] = {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	mesh.vertices[1] = {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	mesh.vertices[2] = {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	mesh.vertices[3] = {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	mesh.indices[0] = 0;
	mesh.indices[1] = 1;
	mesh.indices[2] = 2;
	mesh.indices[3] = 2;
	mesh.indices[4] = 3;
	mesh.indices[5] = 0;

	Entity camera = scene->CreateEntity();
	auto& cameraCameraComponent = camera.Add<CameraComponent>();
	cameraCameraComponent.primary = true;
	cameraCameraComponent.camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
	camera.Add<NativeScriptComponent>().Bind<PerspectiveCameraControllerScript>();

	scene->OnCreate();
}

void SandboxLayer::OnDetach()
{
	GBC_PROFILE_FUNCTION();

	scene->OnDestroy();
}

void SandboxLayer::OnUpdate(Timestep timestep)
{
	GBC_PROFILE_FUNCTION();

	scene->OnUpdate(timestep);
}

void SandboxLayer::OnRender()
{
	GBC_PROFILE_FUNCTION();

#if GBC_ENABLE_STATS
	BasicRenderer::ResetStatistics();
#endif

	scene->OnRender();

#if GBC_ENABLE_STATS
	statistics = BasicRenderer::GetStatistics();
#endif
}

#if GBC_ENABLE_IMGUI
void SandboxLayer::OnImGuiRender()
{
	GBC_PROFILE_FUNCTION();

#if GBC_ENABLE_STATS
	ImGui::Begin("Statistics");
	ImGui::Text("Renderer");
	ImGui::Indent();
	ImGui::Text("Draw Calls:    %d", statistics.drawCalls);
	ImGui::Text("Index Count:   %d", statistics.indexCount);
	ImGui::Text("Vertex Count:  %d", statistics.vertexCount);
	ImGui::Text("Texture Count: %d", statistics.textureCount);
	ImGui::End();
#endif

#if GBC_ENABLE_PROFILE_RUNTIME
	ImGui::Begin("Profiling");
	static bool profiling = false;
	static unsigned int profileCount = 0;
	if (ImGui::Checkbox(profiling ? "Stop Profiling" : "Start Profiling", &profiling))
	{
		if (profiling)
			GBC_PROFILE_BEGIN_RUNTIME("Runtime", (std::string("ProfileSessions/GBCProfileRuntime") += std::to_string(++profileCount)) += ".json");
		else
			GBC_PROFILE_END_RUNTIME();
	}
	ImGui::End();
#endif
}
#endif

void SandboxLayer::OnEvent(Event& event)
{
	GBC_PROFILE_FUNCTION();

	scene->OnEvent(event);
}
