workspace "ml_pgsql"
	configurations { "Debug", "Release" }
	location "build"

project "ml_pgsql"
	kind "SharedLib"
	language "C++"

	cppdialect "C++17"
	files { "src/**.cpp", "src/**.hpp" }

	includedirs {
		"src/",
		"vendor/lua/include",
		"vendor/lua/include/lua",
		"vendor/libpq/include"
	}

	filter { "architecture:x86", "system:windows" }
		defines { "WINDOWS", "WIN32" }
		libdirs { "vendor/lua/lib/x86", "vendor/libpq/lib/x86" }
		links { "lua5.1", "libpq" }

	filter { "architecture:x64", "system:windows" }
		defines { "WINDOWS", "WIN32" }
		libdirs { "vendor/lua/lib/x64", "vendor/libpq/lib/x64" }
		links { "lua5.1", "libpq" }

	filter { "architecture:x86", "system:linux" }
		defines { "LINUX" }
		links { "pq" }

	filter { "architecture:x64", "system:linux" }
		defines { "LINUX" }
		links { "pq" }

	filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

