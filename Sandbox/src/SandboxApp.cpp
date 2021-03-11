#include "gbcpch.h"
#include "SandboxApp.h"
#include "SandboxLayer.h"
#include "GBC/Core/EntryPoint.h"

SandboxApp::SandboxApp()
{
	PushLayer(new SandboxLayer());
}

gbc::Application* gbc::CreateApplication()
{
	return new SandboxApp();
}
