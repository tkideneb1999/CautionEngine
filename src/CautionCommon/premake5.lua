project "CautionCommon"
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
        "**.hpp",
        "**.cpp",
        "**.inl"
    }

    includedirs
    {
        "../CautionCommon"
    }

    filter "system:windows"
    systemversion "latest"
    defines
    {
        "WINDOWS",
        "WIN32",
        "CAUTION_COMMON_EXPORT",
    }

    filter "configurations:Debug"
    runtime "Debug"
    defines 
    {
        "DEBUG",
    }
    symbols "On"
    inlining("Auto")
        
    filter "configurations:Release"
    runtime "Release"
    defines 
    {
        "RELEASE",
    }
    optimize "On"
    inlining("Auto")