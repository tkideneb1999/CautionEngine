project "CautionEngine.Common"
    -- StaticLib = .lib
    -- SharedLib = .dll
    kind "SharedLib"

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
    }

    pchheader "stdafx.h"
    pchsource "stdafx.cpp"

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
            "DEBUG",
            "CAUTION_SHARED"
        }
        symbols "On"
        inlining("Auto")
        
    filter "configurations:Release"
        runtime "Release"
        defines 
        {
            "RELEASE",
            "CAUTION_SHARED"
        }
        optimize "On"
        inlining("Auto")