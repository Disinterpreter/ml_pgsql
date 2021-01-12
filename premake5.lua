workspace "ml_pgsql"
	configurations { "Debug", "Release" }
	location { "build" }

project "ml_pgsql"
	kind "SharedLib"
	language "C++"

	cppdialect "C++17"
	files { "src/**.cpp" }

	filter { "platforms:x86" }
		libdirs {
			"vendor/lua/x86"
		}

		filter { "system:windows" }
			defines { "WINDOWS", "WIN32" }
			libdirs { "vendor/libpq/lib/x86" }
			links { "lua5.1.lib", "libpq.lib" }

		filter { "system:linux" }
			defines { "LINUX" }
			links { "pq" }

	filter { "platforms:x64" }
		libdirs {
			"vendor/lua/x64"
		}

		filter { "system:windows" }
			defines { "WINDOWS", "WIN32" }
			libdirs { "vendor/libpq/lib/x64" }
			links { "lua5.1.lib", "libpq.lib" }

		filter { "system:linux" }
			defines { "LINUX" }
			links { "pq" }

	includedirs {
		"src/",
		"vendor/lua/include",
		"vendor/libpq/include"
	}

	filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"

