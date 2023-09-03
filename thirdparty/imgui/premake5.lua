project "ImGui"
    kind "StaticLib"
    language "C++"

    files
    {
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp"
    }

    filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "On"

    filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"