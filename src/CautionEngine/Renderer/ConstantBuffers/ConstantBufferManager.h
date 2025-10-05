#pragma once

#include <unordered_map>
#include <string>

#include "ConstantBuffer.h"

namespace CautionEngine::Rendering::ConstantBuffers
{
	class ConstantBufferManager
	{
	private:
		std::unordered_map<unsigned int, ConstantBuffer*> m_constantBuffers;
		std::unordered_map<std::string, unsigned int> m_nameMapping;
		std::shared_ptr<DescriptorManager> m_pDescriptorManager;
		unsigned int m_numBackbuffers;
		unsigned int m_counter = 0;

		unsigned int CreateBuffer(const ConstantBufferLayout& layout);

	public:
		ConstantBufferManager(unsigned int numBackbuffers, std::shared_ptr<DescriptorManager> pDescriptorManager);
		~ConstantBufferManager();

		unsigned int GetOrCreateBuffer(ConstantBufferLayout& layout);
		int GetBufferID(std::string& name);
		ConstantBuffer* GetBuffer(unsigned int id);
		ConstantBuffer* GetBuffer(std::string& name);

		void UpdateConstantBufferGPUData(int backBufferIndex);
	};
}