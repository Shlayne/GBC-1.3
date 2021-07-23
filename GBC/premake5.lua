project "GBC"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gbcpch.h"
	pchsource "src/gbcpch.cpp"

-- Headless=0 OpenGL=1 Vulkan=2 Direct3D=3 Metal=4
	defines "GBC_RENDERER_API_ID=1"
-- Only link this if GBC_RENDERER_API_ID == 1 (OpenGL)
	links "opengl32.lib"

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"src",
		"%{includedir.glad}",
		"%{includedir.glfw}",
		"%{includedir.imgui}",
		"%{includedir.imguizmo}",
		"%{includedir.spdlog}",
		"%{includedir.yaml}",

		"%{includedir.entt}",
		"%{includedir.glm}",
		"%{includedir.siv}",
		"%{includedir.stb}"
	}

	links {
		"glad",
		"glfw",
		"imgui",
		"imguizmo",
		"stb",
		"yaml"
	}

	filter "system:windows"
		systemversion "latest"
		defines "GBC_PLATFORM_WINDOWS"
-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning
		buildoptions "/wd5105"

	filter "configurations:Debug"
		defines "GBC_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GBC_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GBC_CONFIG_DIST"
		runtime "Release"
		optimize "on"

	filter "system:not windows"
		excludes "src/Platform/System/Windows/**"
