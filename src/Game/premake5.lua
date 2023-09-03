project "Game"
    kind "SharedLib"
    language (lang)
    cppdialect (dialect)
    
    location (solutionlocations)
    
    targetdir (builddir)
    objdir (intermediate)
    
    files {
        "src/Game/**.h",
        "src/Game/**.cpp"
    }
    
    filter "configurations:Debug"
        defines {"DEBUG"}
        symbols "On"
        
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"