#include <cassert>
#include "TcpRingBufferImp.h"


namespace AzNetworking
{

	TcpRingBufferImpl::TcpRingBufferImpl(uint8_t* buffer, uint32_t bufferSize)
		: m_bufferStart(buffer)
		,m_bufferEnd(buffer + bufferSize)
		,m_writePtr(buffer)
		,m_readPtr(buffer)
	{
		;
	}


	uint8_t* TcpRingBufferImpl::ReserverBlockForWrite(uint32_t numBytes)
	{
		if (GetFreeBytes() < numBytes)
		{
			const uint32_t numUsedBytes = GetUsedBytes();
			memmove(m_bufferStart, m_readPtr, numUsedBytes);
			m_writePtr = m_bufferStart + numUsedBytes;
			m_readPtr = m_bufferStart;
		}

		if (GetFreeBytes() < numBytes)
		{
			return nullptr;
		}

		return m_writePtr;
	}

	bool TcpRingBufferImpl::AdvanceWriteBuffer(uint32_t numBytes)
	{
		if (GetFreeBytes() < numBytes)
		{
			return false;
		}

		m_writePtr += numBytes;

		return true;
	}

	bool TcpRingBufferImpl::AdvanceReadBuffer(uint32_t numBytes)
	{
		if (GetUsedBytes() < numBytes)
		{
			return false;
		}

		m_readPtr += numBytes;

		return true;
	}
}
