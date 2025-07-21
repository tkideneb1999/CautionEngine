project "RecklessEditor"
    kind "ConsoleApp"

	local lang = "C++"
	local dialect = "C++20"
	
	local buildname = "%{prj.name}_%{cfg.buildcfg}"
	local solutionlocations = "../../solutions/%{prj.name}"

    local builddir = ("../../bin/")
	local intermediate = ("../../intermediate/")

    language (lang)
    cppdialect (dialect)
    
    location (solutionlocations)
    
    targetdir (builddir)
    objdir (intermediate)
    
    files 
    {
        -- source
        "**.h",
        "**.hpp",
        "**.cpp",
        "**.inl",

        -- imguizmo
        "../../thirdparty/imguizmo/ImGuizmo.h",
        "../../thirdparty/imguizmo/ImGuizmo.cpp",

        -- glm
        "../../thirdparty/glm/glm/**.hpp",
        "../../thirdparty/glm/glm/**.inl",
    }

    includedirs
    {
        -- RecklessEd
        "/",
        "Widgets",
        "../CautionCommon",
        "../CautionEngine",
        "../CautionEngine.Common",
        "../../%{IncludeDirectories.ImGui}",
        "../../%{IncludeDirectories.ImGui_backends}",
        "../../%{IncludeDirectories.ImGuizmo}",
        "../../%{IncludeDirectories.glm}",
        "../../%{IncludeDirectories.json}"
    }

    links
    {
        "ImGui",
        -- "CautionCommon", -- TODO: enable when we already have implementations of CautionCommon
        "CautionEngine", -- TODO: once we have proper API, we can proper link against the engine here...
        (builddir.. "%{Library.caution_engine_lib}"),
        "%{Library.dx_12}",
        "%{Library.dx_compiler}",
        "%{Library.dx_gi}"
    }

    libdirs
    {
        builddir
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
        inlining("Auto")
        
    filter "configurations:Release"
        runtime "Release"
        defines 
        {
            "RELEASE"
        }
        optimize "On"
        inlining("Auto")