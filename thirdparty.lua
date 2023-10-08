-- Include Directories
IncludeDirectories = {}
IncludeDirectories["ImGui"] = "%{wks.location}/thirdparty/imgui"
IncludeDirectories["usd"] = "%{wks.location}/thirdparty/usd"
IncludeDirectories["ImGuizmo"] = "%{wks.location}/thirdparty/imguizmo"
IncludeDirectories["glm"] = "%{wks.location}/thirdparty/glm"

-- Librarian Additional Directories
LibraryDirectories = {}

-- Libraries (*.lib)
Library = {}
Library["dx_12"] = "d3d12.lib"
Library["dx_compiler"] = "d3dcompiler.lib"
Library["dx_gi"] = "dxgi.lib"