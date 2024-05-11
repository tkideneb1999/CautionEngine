include "thirdparty.lua"

workspace "CautionEngine"
	architecture "x64"
	location "solutions"
	startproject "RecklessEditor"
	editorintegration "On"
	
	configurations
	{ 
		"Debug", 
		"Release"
	}

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