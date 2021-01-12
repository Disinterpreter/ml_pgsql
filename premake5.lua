solution "ml_pgsql"
	configurations { "Debug", "Release" }
	location ( "Build" )
	targetdir "Bin/%{cfg.platform}/%{cfg.buildcfg}"
	platforms { "x86", "x64" }
	
	targetprefix ""
	
	pic "On"
	symbols "On"

	filter "system:windows"
		defines { "WINDOWS", "WIN32" }

	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		optimize "On"
		
	include "ml_pgsql"
