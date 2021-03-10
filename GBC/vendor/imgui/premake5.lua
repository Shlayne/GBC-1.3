project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"include/imgui/imconfig.h",
		"include/imgui/imgui.h",
		"include/imgui/imgui_impl_glfw.h",
		"include/imgui/imgui_impl_opengl3.h",
		"include/imgui/imgui_internal.h",
		"include/imgui/imstb_rectpack.h",
		"include/imgui/imstb_textedit.h",
		"include/imgui/imstb_truetype.h",
		"imgui.cpp",
		"imgui_demo.cpp",
		"imgui_draw.cpp",
		"imgui_impl_glfw.cpp",
		"imgui_impl_opengl3.cpp",
		"imgui_tables.cpp",
		"imgui_widgets.cpp"
	}

	includedirs {
		"%{includedir.glad}",
		"%{includedir.glfw}"
	}
	
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
