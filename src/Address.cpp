#include "Address.hpp"
#include "AddressData.hpp"

namespace network {
void AddressData::setAddress(uint32_t _addr) {
  addr_     = _addr;
  protocol_ = Address::LayerProtocol::IPv4;

  //create mapped address, except for a_ == 0 (any)
  a6_64.c[0] = 0;
  if (addr_) {
    a6_32.c[2] = qToBigEndian(0xffff);
    a6_32.c[3] = qToBigEndian(addr_);
  } else {
    a6_64.c[1] = 0;
  }
}

}  // namespace network
