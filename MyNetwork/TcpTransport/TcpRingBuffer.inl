#include "TcpSocket.h"
#include "TcpRingBuffer.h"


namespace AzNetworking
{
	template<uint32_t SIZE>
	inline TcpRingBuffer<SIZE>::TcpRingBuffer()
		: m_impl(m_buffer, static_cast<uint32_t>(m_buffer.size()))
	{
		;
	}


	template<uint32_t SIZE>
	inline uint8_t* TcpRingBuffer<SIZE>::ReserverBlockForWrite(uint32_t numBytes)
	{
		return m_impl.ReserverBlockForWrite(numBytes);
	}

	template<uint32_t SIZE>
	inline uint8_t* TcpRingBuffer<SIZE>::GetReadBufferData() const
	{
		return m_impl.GetReadBufferData();
	}

	template<uint32_t SIZE>
	inline uint32_t TcpRingBuffer<SIZE>::GetReadBufferSize() const
	{
		return m_impl.GetReadBufferSize();
	}

	template<uint32_t SIZE>
	inline bool TcpRingBuffer<SIZE>::AdvanceWriteBuffer(uint32_t numBytes)
	{
		return m_impl.AdvanceWriteBuffer(numBytes);
	}

	template<uint32_t SIZE>
	inline bool TcpRingBuffer<SIZE>::AdvanceReadBuffer(uint32_t numBytes)
	{
		return m_impl.AdvanceReadBuffer(numBytes);
	}
}
