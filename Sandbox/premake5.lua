project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"src",
		"%{wks.location}/GBC/vendor/spdlog/include",
		"%{wks.location}/GBC/src",
		"%{wks.location}/GBC/vendor",
		"%{includedir.glm}",
		"%{includedir.imgui}"
	}

	links {
		"GBC"
	}

	filter "system:windows"
		defines "GBC_PLATFORM_WINDOWS"
		systemversion "latest"

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
