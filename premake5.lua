workspace "Hazel"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] =	"Hazel/vendor/GLFW/include"
IncludeDir["GLAD"] =	"Hazel/vendor/glad/include"
IncludeDir["IMGUI"] =	"Hazel/vendor/imgui"
IncludeDir["GLM"] =		"Hazel/vendor/glm"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/glad"
include "Hazel/vendor/imgui"

project "Hazel"
	location "Hazel"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"HZ_ENABLE_ASSERTS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.GLM}"
	}

	links 
	{ 
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS",
			--"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}




	filter "configurations:Debug"
		defines {"HZ_DEBUG"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines {"HZ_RELEASE"}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines {"HZ_DIST"}
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src",
		"Hazel/vendor",
		"%{IncludeDir.GLM}"
	}

	links
	{
		"Hazel"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines {"HZ_DEBUG"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines {"HZ_RELEASE"}
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines {"HZ_DIST"}
		runtime "Release"
		optimize "on"