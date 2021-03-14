#include "gbcpch.h"
#include "SandboxApplication.h"
#include "SandboxLayer.h"
#include "GBC/Core/EntryPoint.h"

SandboxApplication::SandboxApplication()
{
	PushLayer(new SandboxLayer());
}

gbc::Application* gbc::CreateApplication()
{
	return new SandboxApplication();
}
