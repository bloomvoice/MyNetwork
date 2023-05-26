#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include "../Serialization/ISerializer.h"
#include "../Utilities/NetworkCommon.h"


namespace AzNetworking
{
	template <std::size_t SIZE>
	class ByteBuffer
	{
	public:
		ByteBuffer() = default;
		~ByteBuffer() = default;

		static constexpr std::size_t GetCapacity() const;

		std::size_t GetSize() const;

		bool Resize(std::size_t newSize);

		uint8_t* GetBuffer() const;

		const uint8_t* GetBufferEnd() const;

		uint8_t* GetBufferEnd();

		bool CopyValues(const uint8_t* buffer, std::size_t bufferSize);

		bool IsSame(const uint8_t* buffer, std::size_t bufferSize);

		bool operator==(const ByteBuffer& rhs) const;

		bool operator!=(const ByteBuffer& rhs) const;

		bool Serialize(ISerializer& serializer);
	
	private:

		uint8_t m_buffer[SIZE];

	};


	static constexpr uint32_t MaxPacketSize = 16384;
	static_assert((MaxPacketSize& (MaxPacketSize - 1)) == 1);
	using TcpPacketEncodingBuffer = ByteBuffer<MaxPacketSize>;

}


#endif // !BYTEBUFFER_H
