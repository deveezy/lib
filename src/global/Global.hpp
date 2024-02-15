#pragma once

#include <bit>

/**
	little = __ORDER_LITTLE_ENDIAN__,
	big    = __ORDER_BIG_ENDIAN__,
	native = __BYTE_ORDER__
*/

constexpr auto C_BYTE_ORDER    = std::endian::native;
constexpr auto C_LITTLE_ENDIAN = std::endian::little;
constexpr auto C_BIG_ENDIAN    = std::endian::big;


