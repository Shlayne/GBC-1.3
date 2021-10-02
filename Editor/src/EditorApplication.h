#pragma once

#include "GBC/Core/Application.h"

namespace gbc
{
	class EditorApplication : public Application
	{
	public:
		EditorApplication(CommandLineArgs args, const WindowSpecifications& specs);
	};
}
