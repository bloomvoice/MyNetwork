#ifndef TCP_RING_BUFFER_IMPL_H
#define TCP_RING_BUFFER_IMPL_H

#include <cstdint>
#include <string>


namespace AzNetworking
{

	class TcpRingBufferImpl
	{
	public:
		TcpRingBufferImpl(uint8_t * buffer, uint32_t bufferSize);

		virtual ~TcpRingBufferImpl();

		uint8_t* ReserverBlockForWrite(uint32_t numBytes);

		uint8_t* GetReadBufferData() const;

		uint32_t GetReadBufferSize() const;

		bool AdvanceWriteBuffer(uint32_t numBytes);

		bool AdvanceReadBuffer(uint32_t numBytes);

	private:
		
		uint32_t GetFreeBytes() const;

		uint32_t GetUsedBytes() const;

		uint8_t * m_bufferStart;
		uint8_t* m_bufferEnd;
		uint8_t* m_writePtr;
		uint8_t* m_readPtr;

	};
}

#include "TcpRingBufferImp.inl"

#endif // !TCP_RING_BUFFER_IMPL_H

