#pragma once

#include <cstdint>
class Network {
public:
  enum class RetCode : std::uint8_t { INVALID_IPV4, INVALID_MASK, INVALID_GATEWAY };

private:
};
