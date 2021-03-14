#include "EditorApplication.h"
#include "EditorLayer.h"
#include "GBC/Core/EntryPoint.h"

namespace gbc
{
	EditorApplication::EditorApplication()
	{
		PushLayer(new EditorLayer());
	}

	Application* CreateApplication()
	{
		return new EditorApplication();
	}
}
