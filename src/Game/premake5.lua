project "Game"
    kind "StaticLib"

	local lang = "C++"
	local dialect = "C++20"
	
	local buildname = "%{prj.name}_%{cfg.buildcfg}"
    local solutionlocations = "../../solutions/%{prj.name}"
    
    local builddir = (solutionlocations.. "/bin/" .. buildname)
	local intermediate = (solutionlocations.. "/intermediate/" .. buildname)

    language (lang)
    cppdialect (dialect)
    
    location (solutionlocations)
    
    targetdir (builddir)
    objdir (intermediate)
    
    files 
    {
        "**.h", 
        "**.cpp"
    }

    includedirs
    {
        "src/Game",
        "src/CautionEngine",
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
        inlining("Auto")
        
    filter "configurations:Release"
        runtime "Release"
        defines 
        {
            "RELEASE"
        }
        optimize "On"
        inlining("Auto")