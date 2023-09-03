project "Editor"
    kind "ConsoleApp"
    language (lang)
    cppdialect (dialect)
    
    location (solutionlocations)
    
    targetdir (builddir)
    objdir (intermediate)
    
    files
    {
        "src/Editor/**.h",
        "src/Editor/**.cpp"
    }
    
    filter "configurations:Debug"
        defines 
        {
            "DEBUG"
        }
        symbols "On"
        
    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"