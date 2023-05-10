#ifndef TYPE_SAFE_INTEGRAL_H
#define TYPE_SAFE_INTEGRAL_H

#include "base.h"


#define AZ_TYPE_SAFE_INTEGRAL(TYPE_NAME, BASE_TYPE)                                                                                                 \
    static_assert(std::is_integral<BASE_TYPE>::value, "Underlying type must be an integral");                                                     \
    enum class TYPE_NAME : BASE_TYPE {};                                                                                                            \
    AZ_DEFINE_ENUM_ARITHMETIC_OPERATORS(TYPE_NAME)                                                                                                  \
    AZ_DEFINE_ENUM_RELATIONAL_OPERATORS(TYPE_NAME)                                                                                                  \
    AZ_DEFINE_ENUM_BITWISE_OPERATORS(TYPE_NAME)

#endif // !TYPE_SAFE_INTEGRAL_H

