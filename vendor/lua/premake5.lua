project "Lua"
	language "C++"
	targetname "lua5.1"
	kind "StaticLib"
	
	defines { "LUA_BUILD_AS_DLL" }
	
	vpaths { 
		["Headers"] = "**.h",
		["Sources"] = "**.c",
		["*"] = "premake5.lua"
	}
	
	files {
		"premake5.lua",
		"src/**.c",
		"src/**.h",
	}
