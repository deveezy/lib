#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "Flags.hpp"

namespace network {

struct sockaddr;
class AddressData;

class Address {
public:
  enum class SpecialAddress : std::uint8_t {
    EMPTY,
    BROADCAST,        // The IPv4 broadcast address. Equivalent to HostAddress("255.255.255.255").
    LOCAL_HOST,       // The IPv4 localhost address. Equivalent to HostAddress("127.0.0.1").
    LOCAL_HOST_IPv6,  // The IPv6 localhost address. Equivalent to HostAddress("::1").
    ANY,  // The dual stack any-address. A socket bound with this address will listen on both IPv4 and IPv6 interfaces.
    ANY_IPv4,  // The IPv4 any-address. Equivalent to HostAddress("0.0.0.0"). A socket bound with this address will listen only on IPv4 interfaces.
    ANY_IPv6,  // The IPv6 any-address. Equivalent to HostAddress("::"). A socket bound with this address will listen only on IPv6 interfaces.
  };

  enum class Conversion : std::uint8_t {
    ConvertV4MappedToIPv4     = 1,
    ConvertV4CompatToIPv4     = 2,
    ConvertUnspecifiedAddress = 4,
    ConvertLocalHost          = 8,
    TolerantConversion        = 0xff,

    StrictConversion = 0
  };

  enum class LayerProtocol { UNKNOWN = -1, IPv4, IPv6, ANY_IP };

  Address() = default;
  explicit Address(uint32_t _ip4);
  explicit Address(const sockaddr *_address);
  Address(const Address &copy);
  explicit Address(SpecialAddress _address);
  ~Address();
  Address &operator=(Address &&_other) noexcept {
    swap(_other);
    return *this;
  }
  Address &operator=(const Address &_other);
  Address &operator=(const SpecialAddress &_other);
  void swap(Address &other) noexcept { d_.swap(other.d_); }

  void setAddress(uint32_t _ip4);
  void setAddress(const std::string _ip6);
  void setAddress(const sockaddr *_address);
  void setAddress(SpecialAddress address);

  [[nodiscard]] LayerProtocol getProtocol() const;
  [[nodiscard]] std::string toString() const;
  uint32_t toIPv4Address(bool *_ok = nullptr) const;
  // TODO(44444): toIPv6Address;

  bool isEqual(const Address &_address, Conversion mode = Conversion::TolerantConversion);

  bool operator==(const Address &_address) const;
  bool operator==(SpecialAddress _address) const;

  inline bool operator!=(const Address &_address) const { return !operator==(_address); }
  inline bool operator!=(SpecialAddress _address) const { return !operator==(_address); }
  [[nodiscard]] bool isNull() const;

  [[nodiscard]] bool isLoopback() const;
  [[nodiscard]] bool isGlobal() const;
  [[nodiscard]] bool isLinkLocal() const;
  [[nodiscard]] bool isSiteLocal() const;
  [[nodiscard]] bool isUniqueLocalUnicast() const;
  [[nodiscard]] bool isMulticast() const;
  [[nodiscard]] bool isBroadcast() const;
  [[nodiscard]] bool isPrivateUse() const;

  friend bool operator==(Address::SpecialAddress lhs, const Address &rhs) { return rhs == lhs; }
  friend bool operator!=(Address::SpecialAddress lhs, const Address &rhs) { return rhs != lhs; }

protected:
  friend class AddressData;
  std::shared_ptr<AddressData> d_;
};

}  // namespace network

ENUM_FLAGS(network::Address::Conversion);
