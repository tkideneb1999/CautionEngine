project "ZeroCheck"
    kind "Utility"

    files
    {
        "**.bat",
    }

    includedirs
    {
        "/",
    }

    filter "configurations:Debug"
        runtime "Debug"
        postbuildmessage "generating solutions..."
        postbuildcommands
        {
            "../scripts/generate_solutions_vs2022.bat",
        }
    filter "configurations:Release"
        runtime "Release"
        postbuildmessage "generating solutions..."
        postbuildcommands
        {
            "../scripts/generate_solutions_vs2022.bat",
        }