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
            "cmd %~dp0\\..\\..\\scripts\\generate_solutions_vs2022.bat",
        }