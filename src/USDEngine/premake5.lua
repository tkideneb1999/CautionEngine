project "USDEngine"
    kind "ConsoleApp"

	local lang = "C++"
	local dialect = "C++20"
	
	local buildname = "%{prj.name}_%{cfg.buildcfg}"
	local builddir = ("bin/" .. buildname)
	local intermediate = ("intermediate/" .. buildname)
	local solutionlocations = "../../solutions/%{prj.name}"

    language (lang)
    cppdialect (dialect)
    
    location (solutionlocations)
    
    targetdir (builddir)
    objdir (intermediate)
    
    files 
    {
        "src/USDEngine/**.h", 
        "src/USDEngine/**.cpp",
        "src/USDEngine/**.inl"
    }

    includedirs
    {
        "src/USDEngine",
        "%{IncludeDirectories.ImGui}",
        "%{IncludeDirectories.usd}"
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        defines 
        {
            "DEBUG"
        }
        symbols "On"
        
    filter "configurations:Release"
        runtime "Release"
        defines 
        {
            "RELEASE"
        }
        optimize "On"