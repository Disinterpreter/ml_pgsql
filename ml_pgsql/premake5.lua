project "ml_pgsql"
	language "C++"
	cppdialect "C++17"
	kind "SharedLib"
	targetname "ml_pgsql"
	
	includedirs { "include", ".", "../vendor" }
	libdirs { "../libs" }

	vpaths {
		["Headers/*"] = "include/**.h",
		["Sources/*"] = "src/**.cpp",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"**.cpp",
		"**.h"
	}
			
	filter { "system:windows", "platforms:x86" }
		links { "x86\\lua5.1.lib", "x86\\libpq.lib" }
		
	filter { "system:windows", "platforms:x64" }
		links { "x64\\lua5.1.lib", "x64\\libpq.lib" }
	
	filter "system:linux"
		links { "pq" }
	
	filter "system:not linux"
		excludes { "include/luaimports.h", "src/luaimports.cpp" }
