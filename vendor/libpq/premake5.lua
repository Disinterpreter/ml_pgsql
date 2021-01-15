project "libpq"
	project "libpq"
	language "C++"
	targetname "libpq"
	kind "StaticLib"

	flags {"ExcludeFromBuild"}
	
	vpaths { 
		["Headers"] = "**.h",
		["Sources"] = "**.c",
		["*"] = "premake5.lua"
	}
	
	files {
		"premake5.lua",
		"**.c",
		"**.h",
	}
