#pragma once

#include <CautionDefinitions.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "ShaderData.h"

namespace CautionEngine::Rendering::ShaderHeaderFile
{
	static inline bool GetKeyAndValue(std::string* str, std::pair<std::string, std::string>* pKV)
	{
		std::pair<std::string, std::string> kv{};

		std::stringstream sstream{ *str };

		//TODO: This does not work if curly braces are allowed to be in the same line -> there can be multiple kv pairs per line
		std::getline(sstream, kv.first, '=');
		std::getline(sstream, kv.second, '=');

		if (kv.first.empty() || kv.second.empty())
			return false;
		return true;
	}

	static inline bool HandleSHFBool(std::string* value, bool* pOutBool)
	{
		if (!value->compare(std::string("True")))
			*pOutBool = true;
		else if (!value->compare(std::string("False")))
			*pOutBool = false;
		else
			return false;
		return true;
	}

	static inline void RemoveChars(std::string* str, std::vector<char> toRemove)
	{
		while (toRemove.size() > 0)
		{
			std::vector<size_t> notFound{};
			for (int i = 0; i < toRemove.size(); i++)
			{
				// Find and index at which character occurs
				size_t index = str->find(toRemove[i]);
				if (index == std::string::npos)
				{
					notFound.push_back(i);
					continue;
				}

				// Erase Character
				str->erase(index, 1);

				// Check if still more characters exist of it
				if (str->find(toRemove[i]) == std::string::npos)
					notFound.push_back(i);
			}

			// Remove chars from list to mark them as done
			for (int i = 0; i < notFound.size(); i++)
			{
				toRemove.erase(toRemove.begin() + notFound[i]);
			}
		}
	}

	static inline unsigned int GetCharAmount(std::string* str, char c)
	{
		int count = 0;
		for (int i = 0; i < str->size(); i++)
		{
			if ((*str)[i] == c)
				count++;
		}
	}

	static inline bool GetSHFBracesContent(
		std::ifstream* fileStream, std::string value, std::vector<std::pair<std::string, std::string>>* bracesContent
	)
	{
		//Check for Curly braces in value or next line
		if (!value.starts_with('{'))
		{
			std::getline(*fileStream, value);
			RemoveChars(&value, std::vector<char>{' ', '\t'});
		}
		if (!value.starts_with('{'))
		{
			std::cout << "No Brace found in the same or the following line" << std::endl;
			return false;
		}

		unsigned int openBraces = 1;
		unsigned int closeBraces = 0;

		if (value.ends_with('}')) 
		{
			closeBraces++;
		}

		// Only one or no keys
		if (closeBraces == openBraces)
		{
			//Erase first curly braces
			value.erase(0, 1);
			value.erase(value.size() - 1, 1);
			std::pair<std::string, std::string> kv;
			if(GetKeyAndValue(&value, &kv))
				bracesContent->push_back(kv);
			return;
		}

		std::pair<std::string, std::string> kv;
		if(GetKeyAndValue())

		std::vector<std::string> lines{};
		while (openBraces != closeBraces)
		{
			if(!std::getline())
		}
	}

	static inline void HandleFillMode(std::string* value, PSOInfo* psoInfo)
	{
		if (!value->compare(std::string("Solid")))
			psoInfo->fillMode = SHADER_FILL_SOLID;
		else if (!value->compare(std::string("Wireframe")))
			psoInfo->fillMode = SHADER_FILL_WIREFRAME;
		else
			std::cout << "Could not determine FillMode, using default" << std::endl;
	}

	static inline void HandleCullMode(std::string* value, PSOInfo* psoInfo)
	{
		if (!value->compare(std::string("Back")))
			psoInfo->cullMode = SHADER_CULL_BACK;
		else if (!value->compare(std::string("Front")))
			psoInfo->cullMode = SHADER_CULL_FRONT;
		else if (!value->compare(std::string("None")))
			psoInfo->cullMode = SHADER_CULL_NONE;
		else
			std::cout << "Could not determine CullMode, using default" << std::endl;
	}

	static inline void HandleTopologyType(std::string* value, PSOInfo* psoInfo)
	{
		if (!value->compare(std::string("Triangle")))
			psoInfo->topologyType = SHADER_TOPOLOGY_TRIANGLE;
		else if (!value->compare(std::string("Line")))
			psoInfo->topologyType = SHADER_TOPOLOGY_LINE;
		else if (!value->compare(std::string("Point")))
			psoInfo->topologyType = SHADER_TOPOLOGY_POINT;
		else if (!value->compare(std::string("Patch")))
			psoInfo->topologyType = SHADER_TOPOLOGY_PATCH;
		else
			std::cout << "Could not determine TopologyType, using default" << std::endl;
	}

	static inline void HandleBlendState(std::string* value, PSOInfo* psoInfo, std::ifstream* fileStream)
	{
		if (value->empty())
		{
			while (std::getline(*fileStream, *value))
			{
				if (value->starts_with("{"))
					break;
			}
		}
	}

	static inline bool ReadFile(LPCWSTR filePath, PSOInfo* psoInfo) 
	{
		std::wstring fp{ filePath };
		size_t extPoit = fp.find_last_of('.');
		fp = fp.substr(0, extPoit);
		fp.append(L".shf");


		std::ifstream fileStream(fp);
		if (!fileStream.is_open())
			return false;

		*psoInfo = {};
		std::string line;

		bool fillModeSet = false;
		bool cullModeSet = false;
		bool conservativeRasterSet = false;
		bool topologyTypeSet = false;
		bool independentBlendSet = false;
		bool BlendStateSet = false;


		
		while (std::getline(fileStream, line))
		{
			std::string test{ " b\ta  ck" };
			RemoveChars(&line, std::vector<char>{' ', '\t'});
			// Ignore comments
			if (line.starts_with('#') || line.empty())
				continue;
			std::stringstream sstream{ line };

			std::pair<std::string, std::string> kv;
			if (!GetKeyAndValue(&line, &kv))
				continue;

			if (!kv.first.compare("FillMode"))
			{
				if (!fillModeSet)
				{
					HandleFillMode(&kv.second, psoInfo);
					fillModeSet = true;
				}
			}
			else if (!kv.first.compare(std::string("CullMode")))
			{
				if (!cullModeSet)
				{
					HandleCullMode(&kv.second, psoInfo);
					cullModeSet = true;
				}
			}
			else if (!kv.first.compare(std::string("ConservativeRaster")))
			{
				if (!conservativeRasterSet)
				{
					if (!HandleSHFBool(&kv.second, &psoInfo->useConservativeRaster))
						std::cout << "Could not determine ConservativeRaster, using default" << std::endl;
					conservativeRasterSet = true;
				}
			}
			else if (!kv.first.compare(std::string("TopologyType")))
			{
				if (!topologyTypeSet)
				{
					HandleTopologyType(&kv.second, psoInfo);
					topologyTypeSet = true;
				}
			}
			else if (!kv.first.compare(std::string("IndependentBlendEnable")))
			{
				if (!independentBlendSet)
				{
					if (!HandleSHFBool(&kv.second, &psoInfo->useConservativeRaster))
						std::cout << "Could not determine IndependentBlendEnable, using default" << std::endl;
					independentBlendSet = true;
				}
			}
			else if (!kv.first.compare(std::string("BlendState")))
			{
				std::cout << "BlendState not yet implemented" << std::endl;
			}
			else
				std::cout << "\"" << kv.first << "\"" << " not recognized, ignoring" << std::endl;
		}

		fileStream.close();
	}
}