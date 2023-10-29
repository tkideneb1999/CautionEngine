project "CautionEngine"
    kind "StaticLib" -- Still not decided yet if we use .lib or .dll

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
        "**.h", 
        "**.cpp",
        "**.inl",

        -- glm
        "../../thirdparty/glm/glm/**.hpp",
        "../../thirdparty/glm/glm/**.inl",
    }

    links
    {
        "%{Library.dx_12}",
        "%{Library.dx_compiler}",
        "%{Library.dx_gi}",
        "../../%{Library.usd_utils}",
        "../../%{Library.usd_imaging}",
        "../../%{Library.usd_geom}",
        "../../%{Library.usd_mtlx}"
    }

    includedirs
    {
        "src/CautionEngine",
        "../../%{IncludeDirectories.glm}",
        "../../%{IncludeDirectories.usd}"
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