include "vendor/premake/solution_items.lua"
include "Dependencies.lua"

workspace "GBC-1.3"
	architecture "x86_64"
	startproject "Editor"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	solution_items {
		"premake5.lua",
		"Dependencies.lua",
		".editorconfig",
		".gitignore",
		".gitattributes"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "GBC/vendor/glad"
	include "GBC/vendor/glfw"
	include "GBC/vendor/imgui"
	include "GBC/vendor/imguizmo"
	include "GBC/vendor/stb"
	include "GBC/vendor/yaml"
group ""

include "GBC"
include "Editor"
