project "assimp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"code/**.h",
		"code/**.hpp",
		"code/**.inl",
		"code/**.cpp",
		"code/**.cc",
		"code/**.c",
		"contrib/**.h",
		"contrib/**.hpp",
		"contrib/**.inl",
		"contrib/**.cpp",
		"contrib/**.cc",
		"contrib/**.c",
		"include/**.h",
		"include/**.hpp",
		"include/**.inl",
		"include/**.cpp",
		"include/**.cc",
		"include/**.c",
	}

	includedirs {
		"code",
		"contrib",
		"include",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"
	}

	filter "files:code/Importer/StepFile/StepFileGen1.cpp"
		buildoptions "/bigobj"

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
