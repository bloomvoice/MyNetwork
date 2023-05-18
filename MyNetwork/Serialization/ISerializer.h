#ifndef ISERIALIZATION_H
#define ISERIALIZATION_H
#include <limits>
#include "../Utilities/NetworkCommon.h"

namespace AzNetworking
{
	class IBitset;
	enum class SerializerMode
	{
		ReadFromOBject,
		WriteFromOBject
	};

	class ISerializer
	{
	public:
		ISerializer() = default;
		virtual ~ISerializer() = default;

		void Invalidate();

		virtual SerializerMode GetSerializerMode() const = 0;

		virtual bool Serialize(bool& value, const char* name) = 0;

		virtual bool Serialize(char& value, const char* name, char minValue = std::numeric_limits<char>::min(), char maxValue = std::numeric_limits<char>::max()) = 0;

		virtual bool Serialize(int8_t& value, const char* name, char minValue = std::numeric_limits<int8_t>::min(), char maxValue = std::numeric_limits<int8_t>::max()) = 0;

		virtual bool Serialize(int16_t& value, const char* name, int16_t minValue = std::numeric_limits<int16_t>::min(), char maxValue = std::numeric_limits<int16_t>::max()) = 0;

		virtual bool Serialize(int32_t& value, const char* name, int32_t minValue = std::numeric_limits<int32_t>::min(), int32_t maxValue = std::numeric_limits<int32_t>::max()) = 0;

		virtual bool Serialize(int64_t& value, const char* name, int64_t minValue = std::numeric_limits<int64_t>::min(), int64_t maxValue = std::numeric_limits<int64_t>::max()) = 0;

#ifdef AZ_TRAIT_COMPILER_INT64_T_IS_LONG
		virtual bool Serialize(s64& value, const char* name, int64_t minValue = std::numeric_limits<int64_t>::min(), int64_t maxValue = std::numeric_limits<int64_t>::max()) = 0;
#endif // AZ_TRAIT_COMPILER_INT64_T_IS_LONG


		virtual bool Serialize(uint8_t& value, const char* name, uint8_t minValue = std::numeric_limits<uint8_t>::min(), uint8_t maxValue = std::numeric_limits<uint8_t>::max()) = 0;

		virtual bool Serialize(uint16_t& value, const char* name, uint16_t minValue = std::numeric_limits<uint16_t>::min(), uint16_t maxValue = std::numeric_limits<uint16_t>::max()) = 0;

		virtual bool Serialize(uint32_t& value, const char* name, uint32_t minValue = std::numeric_limits<uint32_t>::min(), int64_t maxValue = std::numeric_limits<uint32_t>::max()) = 0;

		virtual bool Serialize(uint64_t& value, const char* name, uint64_t minValue = std::numeric_limits<uint64_t>::min(), uint64_t maxValue = std::numeric_limits<uint64_t>::max()) = 0;

#ifdef AZ_TRAIT_COMPILER_INT64_T_IS_LONG
		virtual bool Serialize(u64& value, const char* name, u64 minValue = std::numeric_limits<u64>::min(), int64_t maxValue = std::numeric_limits<u64>::max()) = 0;
#endif 
		virtual bool Serialize(float& value, const char* name, float minValue = std::numeric_limits<float>::min(), int64_t maxValue = std::numeric_limits<float>::max()) = 0;

		virtual bool Serialize(double& value, const char* name, double minValue = std::numeric_limits<double>::min(), int64_t maxValue = std::numeric_limits<double>::max()) = 0;


		virtual bool SerializeBytes(uint8_t* buffer, uint32_t bufferCapacity, bool isString, uint32_t outSize, const char* name) = 0;

		template<typename TYPE>
		bool Serialize(TYPE& value, const char* name);

		virtual bool BeginObject(const char* name) = 0;

		virtual bool EndObject(const char* name) = 0;

		virtual const uint8_t* GetBuffer() const = 0;

		virtual const uint32_t GetCapacity() const = 0;

		virtual const uint32_t GetSize() const = 0;

		virtual void ClearTrackedChangesFlag() = 0;
		
		virtual void ClearTrackedChangesFlag() const = 0;

	protected:
		template<bool IsEnum, bool IsTypeSafeIntegral>
		struct SerializeHelper;

		bool m_serializerValid = 0;


	private:

	};

	ISerializer::ISerializer()
	{
	}

	ISerializer::~ISerializer()
	{
	}
}

#endif // !ISERIALIZATION_H
