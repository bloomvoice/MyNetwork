#ifndef TCP_RING_BUFFER_IMPL_INL
#define TCP_RING_BUFFER_IMPL_INL


namespace AzNetworking
{

	inline uint8_t* TcpRingBufferImpl::GetReadBufferData() const
	{
		return m_readPtr;
	}


	inline uint32_t TcpRingBufferImpl::GetReadBufferSize() const
	{
		return GetUsedBytes();
	}

	inline uint32_t TcpRingBufferImpl::GetFreeBytes() const
	{
		return static_cast<uint32_t>(m_bufferEnd - m_writePtr);
	}

	inline uint32_t TcpRingBufferImpl::GetUsedBytes() const
	{
		return static_cast<uint32_t>(m_writePtr - m_readPtr);
	}
}

#endif //TCP_RING_BUFFER_IMPL_INL