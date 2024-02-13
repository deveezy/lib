#pragma once
#include <cstdint>
#include <initializer_list>
#include <type_traits>

class QFlag {
  int i;

public:
  constexpr inline QFlag(int value) noexcept : i(value) {}
  constexpr inline operator int() const noexcept { return i; }

#if !defined(Q_CC_MSVC)
  // Microsoft Visual Studio has buggy behavior when it comes to
  // unsigned enums: even if the enum is unsigned, the enum tags are
  // always signed
  #if !defined(__LP64__) && !defined(Q_QDOC)
  constexpr inline QFlag(long value) noexcept : i(int(value)) {}
  constexpr inline QFlag(ulong value) noexcept : i(int(long(value))) {}
  #endif
  constexpr inline QFlag(uint32_t value) noexcept : i(int(value)) {}
  constexpr inline QFlag(short value) noexcept : i(int(value)) {}
  constexpr inline QFlag(uint16_t value) noexcept : i(int(uint32_t(value))) {}
  constexpr inline operator uint32_t() const noexcept { return uint32_t(i); }
#endif
};

template <typename Enum>
class QFlags {
  static_assert((sizeof(Enum) <= sizeof(int)),
      "QFlags uses an int as storage, so an enum with underlying "
      "long long will overflow.");
  static_assert((std::is_enum_v<Enum>), "QFlags is only usable on enumeration types.");

public:
  using Int       = std::conditional_t<std::is_unsigned_v<std::underlying_type_t<Enum>>, unsigned int, int>;
  using enum_type = Enum;
  // compiler-generated copy/move ctor/assignment operators are fine!
  constexpr inline QFlags() noexcept : i(0) {}
  constexpr inline explicit QFlags(Enum flags) noexcept : i(Int(flags)) {}
  constexpr inline explicit QFlags(QFlag flag) noexcept : i(flag) {}

  constexpr inline QFlags(std::initializer_list<Enum> flags) noexcept
      : i(initializer_list_helper(flags.begin(), flags.end())) {}

  constexpr static inline QFlags fromInt(Int i) noexcept { return QFlags(QFlag(i)); }
  constexpr inline Int toInt() const noexcept { return i; }

#ifndef QT_TYPESAFE_FLAGS
  constexpr inline QFlags &operator&=(int mask) noexcept {
    i &= mask;
    return *this;
  }
  constexpr inline QFlags &operator&=(uint32_t mask) noexcept {
    i &= mask;
    return *this;
  }
#endif
  constexpr inline QFlags &operator&=(QFlags mask) noexcept {
    i &= mask.i;
    return *this;
  }
  constexpr inline QFlags &operator&=(Enum mask) noexcept {
    i &= Int(mask);
    return *this;
  }
  constexpr inline QFlags &operator|=(QFlags other) noexcept {
    i |= other.i;
    return *this;
  }
  constexpr inline QFlags &operator|=(Enum other) noexcept {
    i |= Int(other);
    return *this;
  }
  constexpr inline QFlags &operator^=(QFlags other) noexcept {
    i ^= other.i;
    return *this;
  }
  constexpr inline QFlags &operator^=(Enum other) noexcept {
    i ^= Int(other);
    return *this;
  }

  constexpr inline explicit operator Int() const noexcept { return i; }
  constexpr inline bool operator!() const noexcept { return !i; }

  constexpr inline QFlags operator|(QFlags other) const noexcept { return QFlags(QFlag(i | other.i)); }
  constexpr inline QFlags operator|(Enum other) const noexcept { return QFlags(QFlag(i | Int(other))); }
  constexpr inline QFlags operator^(QFlags other) const noexcept { return QFlags(QFlag(i ^ other.i)); }
  constexpr inline QFlags operator^(Enum other) const noexcept { return QFlags(QFlag(i ^ Int(other))); }
  constexpr inline QFlags operator&(int mask) const noexcept { return QFlags(QFlag(i & mask)); }
  constexpr inline QFlags operator&(uint32_t mask) const noexcept { return QFlags(QFlag(i & mask)); }
  constexpr inline QFlags operator&(QFlags other) const noexcept { return QFlags(QFlag(i & other.i)); }
  constexpr inline QFlags operator&(Enum other) const noexcept { return QFlags(QFlag(i & Int(other))); }
  constexpr inline QFlags operator~() const noexcept { return QFlags(QFlag(~i)); }

  constexpr inline void operator%(QFlags other) const noexcept = delete;
  constexpr inline void operator+(Enum other) const noexcept   = delete;
  constexpr inline void operator+(int other) const noexcept    = delete;
  constexpr inline void operator-(QFlags other) const noexcept = delete;
  constexpr inline void operator-(Enum other) const noexcept   = delete;
  constexpr inline void operator-(int other) const noexcept    = delete;

  constexpr inline bool testFlag(Enum flag) const noexcept { return testFlags(flag); }
  constexpr inline bool testFlags(QFlags flags) const noexcept {
    return flags.i ? ((i & flags.i) == flags.i) : i == Int(0);
  }
  constexpr inline bool testAnyFlag(Enum flag) const noexcept { return testAnyFlags(flag); }
  constexpr inline bool testAnyFlags(QFlags flags) const noexcept { return (i & flags.i) != Int(0); }
  constexpr inline QFlags &setFlag(Enum flag, bool on = true) noexcept {
    return on ? (*this |= flag) : (*this &= ~QFlags(flag));
  }

  friend constexpr inline bool operator==(QFlags lhs, QFlags rhs) noexcept { return lhs.i == rhs.i; }
  friend constexpr inline bool operator!=(QFlags lhs, QFlags rhs) noexcept { return lhs.i != rhs.i; }
  friend constexpr inline bool operator==(QFlags lhs, Enum rhs) noexcept { return lhs == QFlags(rhs); }
  friend constexpr inline bool operator!=(QFlags lhs, Enum rhs) noexcept { return lhs != QFlags(rhs); }
  friend constexpr inline bool operator==(Enum lhs, QFlags rhs) noexcept { return QFlags(lhs) == rhs; }
  friend constexpr inline bool operator!=(Enum lhs, QFlags rhs) noexcept { return QFlags(lhs) != rhs; }

private:
  constexpr static inline Int initializer_list_helper(typename std::initializer_list<Enum>::const_iterator it,
      typename std::initializer_list<Enum>::const_iterator end) noexcept {
    return (it == end ? Int(0) : (Int(*it) | initializer_list_helper(it + 1, end)));
  }

  Int i;
};

#define Q_DECLARE_FLAGS(Flags, Enum) typedef QFlags<Enum> Flags;

#define Q_DECLARE_OPERATORS_FOR_FLAGS(Flags)                                                           \
  [[maybe_unused]] constexpr inline QFlags<Flags::enum_type> operator|(                                \
      Flags::enum_type f1, Flags::enum_type f2) noexcept {                                             \
    return QFlags<Flags::enum_type>(f1) | f2;                                                          \
  }                                                                                                    \
  [[maybe_unused]] constexpr inline QFlags<Flags::enum_type> operator|(                                \
      Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept {                                     \
    return f2 | f1;                                                                                    \
  }                                                                                                    \
  [[maybe_unused]] constexpr inline QFlags<Flags::enum_type> operator&(                                \
      Flags::enum_type f1, Flags::enum_type f2) noexcept {                                             \
    return QFlags<Flags::enum_type>(f1) & f2;                                                          \
  }                                                                                                    \
  [[maybe_unused]] constexpr inline QFlags<Flags::enum_type> operator&(                                \
      Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept {                                     \
    return f2 & f1;                                                                                    \
  }                                                                                                    \
  [[maybe_unused]] constexpr inline QFlags<Flags::enum_type> operator^(                                \
      Flags::enum_type f1, Flags::enum_type f2) noexcept {                                             \
    return QFlags<Flags::enum_type>(f1) ^ f2;                                                          \
  }                                                                                                    \
  [[maybe_unused]] constexpr inline QFlags<Flags::enum_type> operator^(                                \
      Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept {                                     \
    return f2 ^ f1;                                                                                    \
  }                                                                                                    \
  constexpr inline void operator+(Flags::enum_type f1, Flags::enum_type f2) noexcept         = delete; \
  constexpr inline void operator+(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept = delete; \
  constexpr inline void operator+(int f1, QFlags<Flags::enum_type> f2) noexcept              = delete; \
  constexpr inline void operator-(Flags::enum_type f1, Flags::enum_type f2) noexcept         = delete; \
  constexpr inline void operator-(Flags::enum_type f1, QFlags<Flags::enum_type> f2) noexcept = delete; \
  constexpr inline void operator-(int f1, QFlags<Flags::enum_type> f2) noexcept              = delete; \
  constexpr inline void operator+(int f1, Flags::enum_type f2) noexcept                      = delete; \
  constexpr inline void operator+(Flags::enum_type f1, int f2) noexcept                      = delete; \
  constexpr inline void operator-(int f1, Flags::enum_type f2) noexcept                      = delete; \
  constexpr inline void operator-(Flags::enum_type f1, int f2) noexcept                      = delete;\
