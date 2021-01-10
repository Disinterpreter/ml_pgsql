solution "ml_pgsql"
	configurations { "Debug", "Release" }
	location ( "Build" )
	targetdir "Bin/%{cfg.buildcfg}"

	platforms { "x86", "x64" }
	pic "On"
	symbols "On"

	includedirs { "." }

	filter "system:windows"
		defines { "WINDOWS", "WIN32" }

	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		optimize "On"
		
project "ml_pgsql"
	language "C++"
	cppdialect "C++17"
	kind "SharedLib"
	targetname "ml_pgsql"
	
	includedirs { "include" }
	libdirs { "lib" }

	vpaths {
		["Headers/*"] = "**.h",
		["Sources/*"] = "**.cpp",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"**.cpp",
		"**.h"
	}
			
	filter { "system:windows", "platforms:x86" }
		links { "lua5.1.lib", "libpq.lib" }
		
	filter { "system:windows", "platforms:x64" }
		links { "x64\\lua5.1.lib", "x64\\libpq.lib" }

	filter "system:not linux"
		excludes { "include/luaimports.h", "src/luaimports.cpp" }
