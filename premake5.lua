workspace "GBC-1.3"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["glad"] = "%{wks.location}/GBC/vendor/glad/include"
includedir["glfw"] = "%{wks.location}/GBC/vendor/glfw/include"
includedir["glm"] = "%{wks.location}/GBC/vendor/glm/include"
includedir["imgui"] = "%{wks.location}/GBC/vendor/imgui/include"
includedir["spdlog"] = "%{wks.location}/GBC/vendor/spdlog/include"
-- Header-only; they don't have their own project
includedir["entt"] = "%{wks.location}/GBC/vendor/entt/include"
includedir["siv"] = "%{wks.location}/GBC/vendor/siv/include"
includedir["stb"] = "%{wks.location}/GBC/vendor/stb/include"

group "Dependencies"
	include "GBC/vendor/glad"
	include "GBC/vendor/glfw"
	include "GBC/vendor/glm"
	include "GBC/vendor/imgui"
	include "GBC/vendor/stb"
group ""

include "GBC"
include "Sandbox"
