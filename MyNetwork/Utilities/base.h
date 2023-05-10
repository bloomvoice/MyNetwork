#ifndef BASE_H
#define BASE_H

#include <type_traits>

namespace AZStd
{
	using std::underlying_type;
	using std::underlying_type_t;
}

// Overload arithmetic operators(+, -, *, /, %, <<, >>,++, --) for enum types
#define AZ_DEFINE_ENUM_ARITHMETIC_OPERATORS(EnumType) \
constexpr EnumType operator+(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) + static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType operator-(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) - static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType operator*(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) * static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType operator/(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) / static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType operator%(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) % static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType operator>>(EnumType value, int32_t shift) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(value) >> shift) }; \
    } \
constexpr EnumType operator<<(EnumType value, int32_t shift) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(value) << shift) }; \
    } \
constexpr EnumType& operator+=(EnumType& lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return lhs = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) + static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType& operator-=(EnumType& lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return lhs = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) - static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType& operator*=(EnumType& lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return lhs = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) * static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType& operator/=(EnumType& lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return lhs = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) / static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType& operator%=(EnumType& lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return lhs = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(lhs) % static_cast<UnderlyingType>(rhs)) }; \
    } \
constexpr EnumType& operator>>=(EnumType& value, int32_t shift) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return value = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(value) >> shift) }; \
    } \
constexpr EnumType& operator<<=(EnumType& value, int32_t shift) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return value = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(value) << shift) }; \
    } \
constexpr EnumType& operator++(EnumType& value) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return value = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(value) + 1) }; \
    } \
constexpr EnumType& operator--(EnumType& value) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return value = EnumType{ static_cast<UnderlyingType>(static_cast<UnderlyingType>(value) - 1) }; \
    } \
constexpr EnumType operator++(EnumType& value, int) \
    { const EnumType result = value; ++value; return result; } \
constexpr EnumType operator--(EnumType& value, int) \
    { const EnumType result = value; --value; return result; }



// Overload relational operators(==, !=, <, >, <=, >=) for enum types
#define AZ_DEFINE_ENUM_RELATIONAL_OPERATORS(EnumType) \
constexpr bool operator>(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return static_cast<UnderlyingType>(lhs) > static_cast<UnderlyingType>(rhs); \
    } \
constexpr bool operator<(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return static_cast<UnderlyingType>(lhs) < static_cast<UnderlyingType>(rhs); \
    } \
constexpr bool operator>=(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return static_cast<UnderlyingType>(lhs) >= static_cast<UnderlyingType>(rhs); \
    } \
constexpr bool operator<=(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return static_cast<UnderlyingType>(lhs) <= static_cast<UnderlyingType>(rhs); \
    } \
constexpr bool operator==(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return static_cast<UnderlyingType>(lhs) == static_cast<UnderlyingType>(rhs); \
    } \
constexpr bool operator!=(EnumType lhs, EnumType rhs) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return static_cast<UnderlyingType>(lhs) != static_cast<UnderlyingType>(rhs); \
    }


// Overload bitwise operators(|, &, ^) for enum types
#define AZ_DEFINE_ENUM_BITWISE_OPERATORS(EnumType) \
constexpr EnumType operator | (EnumType a, EnumType b) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType(static_cast<UnderlyingType>(a) | static_cast<UnderlyingType>(b)); \
    } \
constexpr EnumType& operator |= (EnumType &a, EnumType b) \
    { return a = a | b; } \
constexpr EnumType operator & (EnumType a, EnumType b) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType(static_cast<UnderlyingType>(a) & static_cast<UnderlyingType>(b)); \
    } \
constexpr EnumType& operator &= (EnumType &a, EnumType b) \
    { return a = a & b; } \
constexpr EnumType operator ~ (EnumType a) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType(~static_cast<UnderlyingType>(a)); \
    } \
constexpr EnumType operator ^ (EnumType a, EnumType b) \
    { \
        using UnderlyingType = AZStd::underlying_type_t<EnumType>; \
        return EnumType(static_cast<UnderlyingType>(a) ^ static_cast<UnderlyingType>(b)); \
    } \
constexpr EnumType& operator ^= (EnumType &a, EnumType b) \
    { return a = a ^ b; }

#endif // !BASE_H

