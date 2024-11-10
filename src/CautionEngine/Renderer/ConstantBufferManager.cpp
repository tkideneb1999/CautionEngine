#include "stdafx.h"
#include "ConstantBufferManager.h"

namespace CautionEngine::Rendering
{
	unsigned int ConstantBufferManager::CreateBuffer(ConstantBufferLayout& layout)
	{
		ConstantBuffer* pBuffer = new ConstantBuffer(layout, m_numBackbuffers);
		m_constantBuffers.insert(
			std::pair<unsigned int, ConstantBuffer*>(m_counter, pBuffer)
		);
		unsigned int res = m_counter;
		++m_counter;
		return res;
	}

	ConstantBufferManager::ConstantBufferManager(unsigned int numBackbuffers)
		: m_numBackbuffers(numBackbuffers)
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


}