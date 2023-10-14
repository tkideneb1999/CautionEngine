project "RecklessEditor"
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

        -- imguizmo
        "../../thirdparty/imguizmo/ImGuizmo.h",
        "../../thirdparty/imguizmo/ImGuizmo.cpp",

        -- glm
        "../../thirdparty/glm/glm/**.hpp",
        "../../thirdparty/glm/glm/**.inl",
    }

    includedirs
    {
        "../../src/CautionEngine",
        "../../%{IncludeDirectories.ImGui}",
        "../../%{IncludeDirectories.ImGui_backends}",
        "../../%{IncludeDirectories.ImGuizmo}",
        "../../%{IncludeDirectories.glm}"
    }

    links
    {
        "ImGui",
        "CautionEngine",
        "%{Library.dx_12}",
        "%{Library.dx_compiler}",
        "%{Library.dx_gi}"
    }

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "WINDOWS",
            "WIN32"
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