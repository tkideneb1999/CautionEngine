project "CautionEngine"
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
        "**.inl",
        -- glm
        "../../thirdparty/glm/glm/**.hpp",
        "../../thirdparty/glm/glm/**.inl",
    }

    includedirs
    {
        "../CautionEngine",
        "../CautionEngine.Common",
        "../CautionCommon",
        "../../%{IncludeDirectories.glm}",
        "../../%{IncludeDirectories.usd}",
        "../../%{IncludeDirectories.json}"
    }

    links
    {
        -- "CautionCommon", -- TODO: enable when we already have implementations of CautionCommon
        -- "CautionEngine.Common",
        "%{Library.dx_12}",
        "%{Library.dx_compiler}",
        "%{Library.dx_gi}",
        "%{Library.dx_dxcompiler}",
        "../../%{Library.usd_utils}",
        "../../%{Library.usd_imaging}",
        "../../%{Library.usd_geom}",
        "../../%{Library.usd_mtlx}"
    }

    pchheader "stdafx.h"
    pchsource "stdafx.cpp"

    filter "system:windows"
        systemversion "latest"
        defines
        {
            "WINDOWS",
            "WIN32",
            "CAUTION_ENGINE_EXPORT"
        }

        postbuildcommands
        {
            "{COPYFILE} %[../../%{Paths.dxcompiler_bin_x64}/dxcompiler.dll] %[../../bin/dxcompiler.dll]",
            "{COPYFILE} %[../../%{Paths.dxcompiler_bin_x64}/dxcompiler.dll] %[../../bin/dxil.dll]"
        }

        postbuildmessage "Copying directx dependencies..."
    
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