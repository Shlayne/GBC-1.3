VULKAN_SDK = os.getenv("VULKAN_SDK")

includedir = {}
librarydir = {}
library = {}

-- Include Directories
	-- Header and Source; they have their own project.
	includedir["assimp"] = "%{wks.location}/GBC/vendor/assimp/include"
	includedir["box2d"] = "%{wks.location}/GBC/vendor/box2d/include"
	includedir["glad"] = "%{wks.location}/GBC/vendor/glad/include"
	includedir["glfw"] = "%{wks.location}/GBC/vendor/glfw/include"
	includedir["imgui"] = "%{wks.location}/GBC/vendor/imgui/include"
	includedir["imguizmo"] = "%{wks.location}/GBC/vendor/imguizmo/include"
	includedir["stb"] = "%{wks.location}/GBC/vendor/stb/include"
	includedir["yaml"] = "%{wks.location}/GBC/vendor/yaml/include"

	-- Header-Only; they don't have their own project.
	includedir["entt"] = "%{wks.location}/GBC/vendor/entt/include"
	includedir["glm"] = "%{wks.location}/GBC/vendor/glm/include"
	includedir["siv"] = "%{wks.location}/GBC/vendor/siv/include"
	includedir["spdlog"] = "%{wks.location}/GBC/vendor/spdlog/include"

	-- Header and Lib; they also don't have their own project.
	includedir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

-- Library Directories
	-- Vulkan SDK
	librarydir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
	librarydir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Lib"
	librarydir["VulkanSDK_DebugDLL"] = "%{VULKAN_SDK}/Bin"

-- Libraries
	-- Vulkan
	library["Vulkan"] = "%{librarydir.VulkanSDK}/vulkan-1.lib"
	library["VulkanUtils"] = "%{librarydir.VulkanSDK}/VkLayer_utils.lib"

	-- ShaderC Debug
	library["ShaderC_Debug"] = "%{librarydir.VulkanSDK_Debug}/shaderc_sharedd.lib"
	library["SPIRV_Cross_Debug"] = "%{librarydir.VulkanSDK_Debug}/spirv-cross-cored.lib"
	library["SPIRV_Cross_GLSL_Debug"] = "%{librarydir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
	library["SPIRV_Tools_Debug"] = "%{librarydir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

	-- ShaderC Release
	library["ShaderC_Release"] = "%{librarydir.VulkanSDK}/shaderc_shared.lib"
	library["SPIRV_Cross_Release"] = "%{librarydir.VulkanSDK}/spirv-cross-core.lib"
	library["SPIRV_Cross_GLSL_Release"] = "%{librarydir.VulkanSDK}/spirv-cross-glsl.lib"
