workspace "USDEngine"
	architecture "x64"
	configurations { "Debug", "Release" }
	
	local lang = "C++"
	local dialect = "C++20"
	
	local buildname = "%{prj.name}_%{cfg.buildcfg}"
	local builddir = ("bin/" .. buildname)
	local intermediate = ("intermediate/" .. buildname)
	local solutionlocations = "src/solutions/%{prj.name}"
	
	project "USDEngine"
		kind "ConsoleApp"
		language (lang)
		cppdialect (dialect)
		
		location (solutionlocations)
		
		targetdir (builddir)
		objdir (intermediate)
		
		files {
			"src/USDEngine/**.h", 
			"src/USDEngine/**.cpp"
		}
		
		filter "configurations:Debug"
			defines {"DEBUG"}
			symbols "On"
			
		filter "configurations:Release"
			defines {"NDEBUG"}
			optimize "On"
			
	project "Game"
		kind "SharedLib"
		language (lang)
		cppdialect (dialect)
		
		location (solutionlocations)
		
		targetdir (builddir)
		objdir (intermediate)
		
		files {
		    "src/Game/**.h",
			"src/Game/**.cpp"
		}
		
		filter "configurations:Debug"
			defines {"DEBUG"}
			symbols "On"
			
		filter "configurations:Release"
			defines {"NDEBUG"}
			optimize "On"
			
	project "Editor"
		kind "ConsoleApp"
		language (lang)
		cppdialect (dialect)
		
		location (solutionlocations)
		
		targetdir (builddir)
		objdir (intermediate)
		
		files {
		    "src/Editor/**.h",
			"src/Editor/**.cpp"
		}
		
		filter "configurations:Debug"
			defines {"DEBUG"}
			symbols "On"
			
		filter "configurations:Release"
			defines {"NDEBUG"}
			optimize "On"