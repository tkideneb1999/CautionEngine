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

group "Automation"
	include "scripts"
group ""

-- ThirdParty solutions
group "ThirdParty"
	include "thirdparty/imgui"
	include "thirdparty/usd"
group ""

-- Include other projects / solutions
group "RecklessEditor"
	include "src/RecklessEd"
group ""

group "CautionEngine"
	include "src/CautionEngine"
	include "src/CautionEngine.Common"
group ""

group "GamesDLL"
	include "src/Game"
group ""