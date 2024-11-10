#pragma once

#include <unordered_map>
#include <string>

#include "ConstantBuffer.h"

namespace CautionEngine::Rendering
{
	class ConstantBufferManager
	{
	private:
		std::unordered_map<unsigned int, ConstantBuffer*> m_constantBuffers;
		std::unordered_map<std::string, unsigned int> m_nameMapping;
		unsigned int m_numBackbuffers;
		unsigned int m_counter = 0;

		unsigned int CreateBuffer(ConstantBufferLayout& layout);

	public:
		ConstantBufferManager(unsigned int numBackbuffers);
		~ConstantBufferManager();

		unsigned int GetOrCreateBuffer(ConstantBufferLayout& layout);
		int GetBufferID(std::string& name);
		ConstantBuffer* GetBuffer(unsigned int id);
		ConstantBuffer* GetBuffer(std::string& name);
	};
}