#pragma once

#include "Address.hpp"

namespace network {

enum class AddressClassification {
  UNKNOWN   = 0,
  LOOP_BACK = 1,
  LOCAL_NET,
  LINK_LOCAL,
  MULTICAST,
  BROADCAST,

  GLOBAL = 16,
  TEST_NETWORK,
  PRIVATE_NETWORK,
  UNIQUE_LOCAL,
};

class Netmask {
public:
  constexpr Netmask() : length_(255) {}
  bool setAddress(const Address &_address);
  Address address(Address::LayerProtocol protocol);
  int getPrefixLength() const { return length_ == 255 ? -1 : length_; }
  void setPrefixLength(Address::LayerProtocol _proto, int _len) {
    int maxLen = -1;
    if (_proto == Address::LayerProtocol::IPv4) {
      maxLen = 32;
    } else if (_proto == Address::LayerProtocol::IPv6) {
      maxLen = 128;
    }

    if (_len > maxLen || _len < 0) {
      length_ = 255U;
    } else {
      length_ = _len;
    }
  }

  friend bool operator==(Netmask _n1, Netmask _n2) { return _n1.length_ == _n2.length_; }

private:
  uint8_t length_;
};

class AddressData {
  AddressData();
  void setAddress(uint32_t _addr = 0);
  void setAddress(const std::string &_addr);

  bool parse(const std::string &_ipString);
  void clear();

  union {
    struct {
      uint64_t c[2];
    } a6_64;
    struct {
      uint64_t c[4];
    } a6_32;
  };
  uint32_t addr_ {0};  // IPv4 address
  Address::LayerProtocol protocol_ {Address::LayerProtocol::UNKNOWN};

  [[nodiscard]] AddressClassification classify() const;
  static AddressClassification classify(const Address &_addr) { return _addr.d_->classify(); }

  friend class Address;
};
}  // namespace network
