#pragma once

#include <bitset>
#include <cstdint>
#include <type_traits>

//! Enum-based flags false checker
template <typename TEnum>
struct IsEnumFlags : public std::false_type {};

//! Register a new enum-based flags macro
/*!
    Should be used to get access to AND/OR/XOR logical operators with a given enum values.

    Example:
    \code{.cpp}
    enum class MyFlags
    {
        None  = 0x0,
        One   = 0x1,
        Two   = 0x2,
        Three = 0x4,
        Four  = 0x8
    };

    ENUM_FLAGS(MyFlags)

    int main()
    {
        auto mask = MyFlags::One | MyFlags::Two;
        if (mask & MyFlags::Two)
        {
            ...
        }
    }
    \endcode
*/
#define ENUM_FLAGS(type) \
  template <>            \
  struct IsEnumFlags<type> : std::true_type {};

template <typename TEnum>
constexpr inline bool is_enum_flags_v = IsEnumFlags<TEnum>::value;

//! Enum-based flags
/*!
    Helper class for enum based flags which wraps particular enum as a template parameter
    and provides flags manipulation operators and methods.

    Not thread-safe.
*/
template <typename TEnum>
class Flags {
  static_assert((sizeof(TEnum) <= sizeof(uint32_t)),
      "Flags uses an int as storage, so an enum with underlying "
      "long long will overflow.");
  static_assert((std::is_enum_v<TEnum>), "Flags is only usable on enumeration types.");
  //! Enum underlying type
  using type = std::make_unsigned_t<std::underlying_type_t<TEnum>>;

public:
  constexpr inline Flags() noexcept : _value(0) {}
  constexpr inline Flags(type value) noexcept : _value(value) {}
  constexpr inline Flags(TEnum value) noexcept : _value(type(value)) {}
  Flags(const Flags &) noexcept = default;
  Flags(Flags &&) noexcept      = default;
  ~Flags() noexcept             = default;

  // clang-format off
  constexpr Flags &operator=(type value) noexcept { _value = value; return *this; }
  constexpr Flags &operator=(TEnum value) noexcept { _value = type(value); return *this; }

  //! Is any flag set?
  constexpr explicit operator bool() const noexcept { return isset(); }

  //! Is no flag set?
  constexpr bool operator!() const noexcept { return !isset(); }

  //! Reverse all flags
  constexpr inline Flags operator~() const noexcept { return Flags(~_value); }

  //! Flags logical assign operators
  constexpr inline Flags &operator&=(const Flags &flags) noexcept { _value &= flags._value; return *this; }
  constexpr inline Flags &operator&=(TEnum _val) noexcept { _value &= type(_val); return *this; }
  // constexpr inline Flags &operator&=(type _val) noexcept { _value &= _val; return *this; }

  constexpr inline Flags &operator|=(const Flags &flags) noexcept { _value |= flags._value; return *this; }
  constexpr inline Flags &operator|=(TEnum _val) noexcept { _value |= type(_val); return *this; }
  // constexpr inline Flags &operator|=(type _val) noexcept { _value |= _val; return *this; }

  constexpr inline Flags &operator^=(const Flags &flags) noexcept { _value ^= flags._value; return *this; }
  constexpr inline Flags &operator^=(TEnum _val) noexcept { _value ^= type(_val); return *this; }
  // constexpr inline Flags &operator^=(type _val) noexcept { _value ^= _val; return *this; }

  //! Flags logical friend operators
  friend Flags operator&(const Flags &flags1, const Flags &flags2) noexcept { return Flags(flags1._value & flags2._value); }
  friend Flags operator|(const Flags &flags1, const Flags &flags2) noexcept { return Flags(flags1._value | flags2._value); }
  friend Flags operator^(const Flags &flags1, const Flags &flags2) noexcept { return Flags(flags1._value ^ flags2._value); }

  // Flags comparison
  friend bool operator==(const Flags &flags1, const Flags &flags2) noexcept { return flags1._value == flags2._value; }
  friend bool operator!=(const Flags &flags1, const Flags &flags2) noexcept { return flags1._value != flags2._value; }

  //! Convert to the enum value
  explicit operator TEnum() const noexcept { return TEnum(_value); }

  //! Is any flag set?
  [[nodiscard]] constexpr bool isset() const noexcept { return (_value != 0); }
  //! Is the given flag set?
  [[nodiscard]] constexpr bool isset(type value) const noexcept { return (_value & value) != 0; }
  //! Is the given flag set?
  [[nodiscard]] constexpr bool isset(TEnum value) const noexcept { return (_value & (type)value) != 0; }

  //! Get the enum value
  [[nodiscard]] TEnum value() const noexcept { return (TEnum)_value; }
  //! Get the underlying enum value
  [[nodiscard]] type underlying() const noexcept { return _value; }
  //! Get the bitset value
  std::bitset<sizeof(type) * 8> bitset() const noexcept { return {_value}; }

  //! Swap two instances
  void swap(Flags &flags) noexcept {
    using std::swap;
    swap(_value, flags._value);
  }
  template <typename UEnum>
  friend void swap(Flags<UEnum> &flags1, Flags<UEnum> &flags2) noexcept;

private:
  type _value;
};

// clang-format off

template <typename TEnum>
inline void swap(Flags<TEnum> &flags1, Flags<TEnum> &flags2) noexcept {
  flags1.swap(flags2);
}

template <typename TEnum>
constexpr auto operator&(TEnum value1, TEnum value2) noexcept
    -> std::enable_if_t<is_enum_flags_v<TEnum>, Flags<TEnum>> {
  return Flags<TEnum>(value1) & value2;
}

template <typename TEnum>
constexpr auto operator|(TEnum value1, TEnum value2) noexcept
    -> std::enable_if_t<is_enum_flags_v<TEnum>, Flags<TEnum>> {
  return Flags<TEnum>(value1) | value2;
}

template <typename TEnum>
constexpr auto operator^(TEnum value1, TEnum value2) noexcept
    -> std::enable_if_t<is_enum_flags_v<TEnum>, Flags<TEnum>> {
  return Flags<TEnum>(value1) ^ value2;
}

