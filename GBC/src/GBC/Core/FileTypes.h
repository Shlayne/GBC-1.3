#pragma once

#include "GBC/Core/Core.h"
#include <array>
#include <filesystem>

namespace gbc
{
	// TODO: think about moving this one function to Utils::something
	// Converts an std::array<const wchar_t*, Size> to a an std::array<std::wstring_view, Size> at compile time
	template<size_t Size>
	static constexpr std::array<std::wstring_view, Size> WideCharsToWideStringViews(std::wstring_view(&& array)[Size])
	{ return std::to_array(array); }

	namespace FileType
	{
		// Generic extensions
		static constexpr auto textures = WideCharsToWideStringViews({ L".png", L".jpg", L".jpeg", L".jpe", L".jfif" });
		static constexpr auto model3Ds = WideCharsToWideStringViews({ L"3d", L"3ds", L"3mf", L"ac", L"ac3d", L"acc", L"amf", L"ase", L"ask", L"b3d", L"blend", L"bsp", L"bvh", L"cob", L"csm", L"dae", L"dxf", L"enff", L"fbx", L"glb", L"gltf", L"hmp", L"ifc", L"ifczip", L"irr", L"irrmesh", L"lwo", L"lxo", L"lws", L"md2", L"md3", L"md5anim", L"md5camera", L"md5mesh", L"mdc", L"mdl", L"mesh", L"mot", L"ms3d", L"ndo", L"nff", L"obj", L"off", L"ogex", L"pk3", L"ply", L"prj", L"pmx", L"q3o", L"q3s", L"raw", L"scn", L"sib", L"smd", L"stl", L"step", L"stp", L"ter", L"uc", L"vta", L"x", L"x3d", L"x3db", L"xgl", L"zae", L"zgl" });

		// GBC extensions
		static constexpr std::wstring_view gProject = L".gproj";
		static constexpr std::wstring_view gScene = L".gscene";
		static constexpr std::wstring_view gModel3D = L".gobj3";
		static constexpr std::wstring_view gModel4D = L".gobj4";
		static constexpr std::wstring_view gTexture = L".gtex";
	}

	const wchar_t* GetFilter(std::wstring_view extension) noexcept;
	template<size_t Size> const wchar_t* GetFilter(const std::array<std::wstring_view, Size>& extensions) noexcept;

	constexpr bool IsFile(std::wstring_view filepath, std::wstring_view extension) noexcept;
	constexpr bool IsFilepath(const std::filesystem::path& filepath, std::wstring_view extension) noexcept;
	std::filesystem::path AppendType(const std::filesystem::path& filepath, std::wstring_view extension);
	std::filesystem::path& AppendTypeTo(std::filesystem::path& filepath, std::wstring_view extension);

	template<size_t Size> constexpr bool IsFile(std::wstring_view filepath, const std::array<std::wstring_view, Size>& extensions) noexcept;
	template<size_t Size> constexpr bool IsFilepath(const std::filesystem::path& filepath, const std::array<std::wstring_view, Size>& extensions) noexcept;
	template<size_t Size> std::filesystem::path AppendType(const std::filesystem::path& filepath, const std::array<std::wstring_view, Size>& extensions);
	template<size_t Size> std::filesystem::path& AppendTypeTo(std::filesystem::path& filepath, const std::array<std::wstring_view, Size>& extensions);
}

#include "FileTypes.inl"
