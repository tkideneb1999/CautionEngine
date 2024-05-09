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
        "**.cpp",
        "**.inl",

        -- common
        "../CautionCommon/**.h",
        "../CautionCommon/**.cpp",
        "../CautionCommon/**.inl",

        -- imguizmo
        "../../thirdparty/imguizmo/ImGuizmo.h",
        "../../thirdparty/imguizmo/ImGuizmo.cpp",

        -- glm
        "../../thirdparty/glm/glm/**.hpp",
        "../../thirdparty/glm/glm/**.inl",
    }

    includedirs
    {
        "../CautionEngine",
        "../CautionCommon",
        "../../%{IncludeDirectories.ImGui}",
        "../../%{IncludeDirectories.ImGui_backends}",
        "../../%{IncludeDirectories.ImGuizmo}",
        "../../%{IncludeDirectories.glm}"
    }

    links
    {
        "ImGui",
        "CautionEngine",
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