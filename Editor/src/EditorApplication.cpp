#include "EditorApplication.h"
#include "Layers/EditorLayer.h"
#include "GBC/Core/EntryPoint.h"

namespace gbc
{
	EditorApplication::EditorApplication(CommandLineArgs args, const WindowSpecifications& specs)
		: Application(args, specs)
	{
		PushLayer(new EditorLayer());
	}

	Application* CreateApplication(CommandLineArgs args)
	{
		WindowSpecifications specs;
		specs.maximizeOnShow = true;
		specs.title = "GBC v1.3 | Editor";

		return new EditorApplication(args, specs);
	}
}
