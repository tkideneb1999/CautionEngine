include "thirdparty.lua"

workspace "CautionEngine"
	architecture "x64"
	location "solutions"
	startproject "RecklessEditor"
	
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

-- Include other projects / solutions
include "src/Editor"
include "src/CautionEngine"

group "games"
	include "src/Game"
group ""