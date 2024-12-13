#pragma once

#include <CautionEngineDefinitions.h>

#include <string>
#include <vector>

#include "ShaderData.h"

namespace CautionEngine::Rendering
{
	class D3D12ConstantBufferVariable
	{
	public:
		std::string name;
		ShaderVariableTypes type;
		unsigned short int count;
		unsigned short int offset;
	};

	class D3D12ConstantBufferLayout 
	{
	public:
		std::vector<D3D12ConstantBufferVariable> elements;
		std::string name;
	};
}