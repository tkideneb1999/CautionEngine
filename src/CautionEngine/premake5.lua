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
        "../../%{IncludeDirectories.glm}",
        "../../%{IncludeDirectories.usd}",
        "../../%{IncludeDirectories.json}"
    }

    links
    {
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
            "WIN32"
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