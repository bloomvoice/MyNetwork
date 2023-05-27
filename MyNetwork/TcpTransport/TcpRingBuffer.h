#ifndef TCP_RING_BUFFER_H
#define TCP_RING_BUFFER_H

#include <cstdint>
#include "../Utilities/NetworkCommon.h"
#include "TcpRingBufferImp.h"

namespace AzNetworking
{

	template<uint32_t SIZE>
	class TcpRingBuffer
	{
	public:
		TcpRingBuffer();


		virtual ~TcpRingBuffer() = default;

		uint8_t * ReserverBlockForWrite(uint32_t numBytes);

		uint8_t* GetReadBufferData() const;

		uint32_t GetReadBufferSize() const;

		bool AdvanceWriteBuffer(uint32_t numBytes);

		bool AdvanceReadBuffer(uint32_t numBytes);


	private:
		
		uint8_t m_buffer[SIZE];

		TcpRingBufferImpl m_impl;
	};
}

#include "TcpRingBuffer.inl"

#endif // !TCP_RING_BUFFER_H

