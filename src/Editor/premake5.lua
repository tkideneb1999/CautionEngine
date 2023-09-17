project "USDEngine.Editor"
    kind "ConsoleApp"

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
        "**.cpp",

        "../../thirdparty/imguizmo/ImGuizmo.h",
        "../../thirdparty/imguizmo/ImGuizmo.cpp"
    }

    includedirs
    {
        "../",
        "../../thirdparty/imgui",
        "../../thidparty/imguizmo",
        "%{IncludeDirectories.usd}"
    }

    links
    {
        -- "ImGui",
        "USDEngine"
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