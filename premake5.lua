include "thirdparty.lua"

workspace "USDEngine"
	architecture "x64"
	location "solutions"
	startproject "USDEngine.Editor"
	
	configurations
	{ 
		"Debug", 
		"Release"
	}
	
	local lang = "C++"
	local dialect = "C++20"
	
	local buildname = "%{prj.name}_%{cfg.buildcfg}"
	local builddir = ("bin/" .. buildname)
	local intermediate = ("intermediate/" .. buildname)
	local solutionlocations = "src/solutions/%{prj.name}"

-- ThirdParty solutions
group "thirdparty"
	include "thirdparty/imgui"
	include "thirdparty/usd"
group ""

include "src/Editor"
include "src/Game"
include "src/USDEngine"

