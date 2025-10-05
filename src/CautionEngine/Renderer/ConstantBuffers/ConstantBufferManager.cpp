#include "stdafx.h"
#include "ConstantBufferManager.h"

namespace CautionEngine::Rendering::ConstantBuffers
{
	unsigned int ConstantBufferManager::CreateBuffer(const ConstantBufferLayout& layout)
	{
		ConstantBuffer* pBuffer = new ConstantBuffer(layout, m_numBackbuffers);
		unsigned int res = m_counter;
		m_constantBuffers.insert(
			std::pair<unsigned int, ConstantBuffer*>(res, pBuffer)
		);
		m_nameMapping.insert(
			std::pair<std::string, unsigned int>(layout.GetName(), res)
		);
		pBuffer->Init(m_pDescriptorManager);
		
		++m_counter;
		return res;
	}

	ConstantBufferManager::ConstantBufferManager(unsigned int numBackbuffers, std::shared_ptr<DescriptorManager> pDescriptorManager)
		: m_numBackbuffers(numBackbuffers)
		, m_pDescriptorManager(pDescriptorManager)
	{
	}

	ConstantBufferManager::~ConstantBufferManager()
	{
		for (auto& pair : m_constantBuffers)
		{
			delete pair.second;
		}
	}

	unsigned int ConstantBufferManager::GetOrCreateBuffer(ConstantBufferLayout& layout)
	{
		// Check if Buffer already exists, and return ID, if yes
		// TODO: Make this thread safe
		auto nameFinder = m_nameMapping.find(layout.GetName());
		if (nameFinder != m_nameMapping.end())
		{
			unsigned int id = nameFinder->second;
			
			return nameFinder->second;
		}

		return CreateBuffer(layout);
	}

	int ConstantBufferManager::GetBufferID(std::string& name)
	{
		auto idFinder = m_nameMapping.find(name);
		if (idFinder != m_nameMapping.end())
		{
			return idFinder->second;
		}
		return -1;
	}

	ConstantBuffer* ConstantBufferManager::GetBuffer(unsigned int id)
	{
		auto bufferFinder = m_constantBuffers.find(id);
		if (bufferFinder != m_constantBuffers.end())
		{
			return bufferFinder->second;
		}
		return nullptr;
	}

	ConstantBuffer* ConstantBufferManager::GetBuffer(std::string& name)
	{
		int id = GetBufferID(name);
		if (id < 0)
		{
			return nullptr;
		}
		return GetBuffer(id);
	}

	void ConstantBufferManager::UpdateConstantBufferGPUData(int backBufferIndex)
	{
		for (auto& buffer : m_constantBuffers)
		{
			buffer.second->UpdateGPUMemory(backBufferIndex);
		}
	}


}