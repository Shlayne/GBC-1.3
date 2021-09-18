#include "EditorApplication.h"
#include "EditorLayer.h"
#include "GBC/Core/EntryPoint.h"

namespace gbc
{
	EditorApplication::EditorApplication(const WindowSpecifications& specs)
		: Application(specs)
	{
		PushLayer(new EditorLayer());
	}

	Application* CreateApplication(CommandLineArgs args)
	{
		WindowSpecifications specs;
		specs.maximizeOnShow = true;

		return new EditorApplication(specs);
	}
}
