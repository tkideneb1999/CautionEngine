include "thirdparty.lua"

workspace "USDEngine"
	architecture "x64"
	configurations { "Debug", "Release" }
	
	local lang = "C++"
	local dialect = "C++20"
	
	local buildname = "%{prj.name}_%{cfg.buildcfg}"
	local builddir = ("bin/" .. buildname)
	local intermediate = ("intermediate/" .. buildname)
	local solutionlocations = "src/solutions/%{prj.name}"