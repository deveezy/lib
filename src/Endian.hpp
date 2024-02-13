#pragma once

#include <cstdint>
#include <type_traits>

inline constexpr uint64_t qbswap_helper(uint64_t _source) {
  // clang-format off
    return 0U
        | ((_source & uint64_t(0x00000000000000ff)) << 56U)
        | ((_source & uint64_t(0x000000000000ff00)) << 40U)
        | ((_source & uint64_t(0x0000000000ff0000)) << 24U)
        | ((_source & uint64_t(0x00000000ff000000)) << 8U)
        | ((_source & uint64_t(0x000000ff00000000)) >> 8U)
        | ((_source & uint64_t(0x0000ff0000000000)) >> 24U)
        | ((_source & uint64_t(0x00ff000000000000)) >> 40U)
        | ((_source & uint64_t(0xff00000000000000)) >> 56U);
  // clang-format on
}

inline constexpr uint32_t qbswap_helper(uint32_t _source) {
  // clang-format off
    return 0U
        | ((_source & 0x000000ff) << 24U)
        | ((_source & 0x0000ff00) << 8U)
        | ((_source & 0x00ff0000) >> 8U)
        | ((_source & 0xff000000) >> 24U);
  // clang-format on
}

inline constexpr uint16_t qbswap_helper(uint16_t _source) {
  return uint16_t(0U | ((_source & 0x00ff) << 8) | ((_source & 0xff00) >> 8));
}

inline constexpr uint8_t qbswap_helper(uint8_t _source) { return _source; }

// Used to implement a type-safe and alignment-safe copy operation
// If you want to avoid the memcpy, you must write specializations for these functions
template <typename T>
Q_ALWAYS_INLINE void qToUnaligned(const T src, void *dest) {
  // Using sizeof(T) inside memcpy function produces internal compiler error with
  // MSVC2008/ARM in tst_endian -> use extra indirection to resolve size of T.
  const std::size_t size = sizeof(T);
#if __has_builtin(__builtin_memcpy)
  __builtin_memcpy
#else
  memcpy
#endif
      (dest, &src, size);
}

/*
 * T qbswap(T source).
 * Changes the byte order of a value from big-endian to little-endian or vice versa.
 * This function can be used if you are not concerned about alignment issues,
 * and it is therefore a bit more convenient and in most cases more efficient.
*/
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline constexpr T qbswap(T source) {
  return T(qbswap_helper(std::make_unsigned_t<T>(source)));
}

/*
 * qbswap(const T src, const void *dest);
 * Changes the byte order of \a src from big-endian to little-endian or vice versa
 * and stores the result in \a dest.
 * There is no alignment requirements for \a dest.
*/
template <typename T>
inline void qbswap(const T src, void *dest) {
  qToUnaligned<T>(qbswap(src), dest);
}
