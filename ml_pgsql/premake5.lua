project "ml_pgsql"
	language "C++"
	cppdialect "C++17"
	kind "SharedLib"
	targetname "ml_pgsql"
	
	includedirs { ".", "../vendor", "../vendor/sparsehash/src" }
	libdirs { "../libs" }

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
		links { "Lua", "x86\\libpq.lib" }
		
	filter { "system:windows", "platforms:x64" }
		links { "Lua", "x64\\libpq.lib" }
	
	filter "system:linux"
		links { "pq" }
	
	filter "system:not linux"
		excludes { "lua/luaimports.h", "lua/luaimports.cpp" }
