-- Include Directories
IncludeDirectories = {}
IncludeDirectories["ImGui"] = "thirdparty/imgui"
IncludeDirectories["ImGui_backends"] = "thirdparty/imgui/backends"
IncludeDirectories["usd"] = "thirdparty/usd/bin/include"
IncludeDirectories["ImGuizmo"] = "thirdparty/imguizmo"
IncludeDirectories["glm"] = "thirdparty/glm"
IncludeDirectories["json"] = "thirdparty/libraries/json"

-- Librarian Additional Directories
LibraryDirectories = {}
LibraryDirectories["usd_dir"] = "%{wks.location}/thirdparty/usd/bin/lib"

-- Libraries (*.lib)
Library = {}

Library["caution_engine_lib"] = "CautionEngine.lib"

Library["dx_12"] = "d3d12.lib"
Library["dx_compiler"] = "d3dcompiler.lib"
Library["dx_gi"] = "dxgi.lib"

Library["usd_imaging"] = "thirdparty/usd/bin/lib/usd_usdImaging.lib"
Library["usd_utils"] = "thirdparty/usd/bin/lib/usd_usdUtils.lib"
Library["usd_media"] = "thirdparty/usd/bin/lib/usd_usdMedia.lib"
Library["usd_geom"] = "thirdparty/usd/bin/lib/usd_usdGeom.lib"
Library["usd_mtlx"] = "thirdparty/usd/bin/lib/usd_usdMtlx.lib"