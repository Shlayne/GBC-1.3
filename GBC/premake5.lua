project "GBC"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "gbcpch.h"
	pchsource "src/gbcpch.cpp"

	files {
		"src/**.h",
		"src/**.cpp"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
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
		"yaml",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines "GBC_PLATFORM_WINDOWS"

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
