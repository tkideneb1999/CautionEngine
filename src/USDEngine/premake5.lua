project "USDEngine"
    kind "ConsoleApp"
    language (lang)
    cppdialect (dialect)
    
    location (solutionlocations)
    
    targetdir (builddir)
    objdir (intermediate)
    
    files {
        "src/USDEngine/**.h", 
        "src/USDEngine/**.cpp"
    }
    
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
        
    filter "configurations:Release"
        defines {"NDEBUG", "RELEASE"}
        optimize "On"