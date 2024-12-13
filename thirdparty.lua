-- Include Directories
IncludeDirectories = {}
IncludeDirectories["ImGui"] = "thirdparty/imgui"
IncludeDirectories["ImGui_backends"] = "thirdparty/imgui/backends"
IncludeDirectories["usd"] = "thirdparty/usd/bin/include"
IncludeDirectories["ImGuizmo"] = "thirdparty/imguizmo"
IncludeDirectories["glm"] = "thirdparty/glm"
IncludeDirectories["json"] = "thirdparty/libraries/json"

-- Paths
Paths = {}
Paths["dxcompiler_bin_x64"] = "thirdparty/libraries/dxcompiler/bin/x64"

-- Librarian Additional Directories
LibraryDirectories = {}
LibraryDirectories["usd_dir"] = "%{wks.location}/thirdparty/usd/bin/lib"
LibraryDirectories["dxcompiler"] = "%{wks.location}/thirdparty/libraries/dxcompiler/lib"

-- Libraries (*.lib)
Library = {}

Library["caution_common_lib"] = "CautionCommon.lib"
Library["caution_engine_lib"] = "CautionEngine.lib"
Library["caution_engine_common_lib"] = "CautionEngine.Common.lib"

Library["dx_12"] = "d3d12.lib"
Library["dx_compiler"] = "d3dcompiler.lib"
Library["dx_gi"] = "dxgi.lib"
Library["dx_dxcompiler"] = "dxcompiler.lib"

Library["usd_imaging"] = "thirdparty/usd/bin/lib/usd_usdImaging.lib"
Library["usd_utils"] = "thirdparty/usd/bin/lib/usd_usdUtils.lib"
Library["usd_media"] = "thirdparty/usd/bin/lib/usd_usdMedia.lib"
Library["usd_geom"] = "thirdparty/usd/bin/lib/usd_usdGeom.lib"
Library["usd_mtlx"] = "thirdparty/usd/bin/lib/usd_usdMtlx.lib"