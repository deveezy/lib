#include <iostream>
#include <net/if.h>
#include <string>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include "Flags.hpp"
#include <ifaddrs.h>

void setIpV4(const std::string &_iface, const std::string &_ip) {
  if (_iface.empty()) {
    fprintf(stderr, "Incorrect interface.\n");
    return;
  }

  int sock = 0;
  sock     = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    fprintf(stderr, "Could not get socket.\n");
    return;
  }

  ifreq ifr;
  sockaddr_in sin;

  snprintf(ifr.ifr_name, IFNAMSIZ, "%s", _iface.c_str());  // ??  _iface.length()
  if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
    fprintf(stderr, "ifdown: shutdown ");
    perror(ifr.ifr_name);
    return;
  }
#ifdef ifr_flags
  #define IRFFLAGS ifr_flags
#else /* Present on kFreeBSD */
  #define IRFFLAGS ifr_flagshigh
#endif

  // If interface is down, bring it up
  if (!(ifr.IRFFLAGS & IFF_UP)) {
    fprintf(stdout, "Device is currently down..setting up.-- %u\n", ifr.IRFFLAGS);
    ifr.IRFFLAGS |= IFF_UP;
    if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
      fprintf(stderr, "ifup: failed ");
      perror(ifr.ifr_name);
      return;
    }
  }

  sin.sin_family = AF_INET;

  // Convert IP from numbers and dots to binary notation
  inet_aton(_ip.c_str(), &sin.sin_addr);
  std::memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));

  // Set interface address
  if (ioctl(sock, SIOCSIFADDR, &ifr) < 0) {
    fprintf(stderr, "Cannot set IP address. ");
    perror(ifr.ifr_name);
    return;
  }
#undef IRFFLAGS
}

struct Addr4Entry {
  uint32_t ipv4_;
  uint32_t gateway_;
  uint32_t netmask_;
};

class Interface {
public:
  explicit Interface(std::string &&_name) {
    // TODO(44444): fill entries
  }

private:
  std::string name_;
  // std::vector<Row4> rows_;
};

namespace net {
std::vector<Interface> getIfacesNames() {
  std::vector<Interface> list;
  struct ifaddrs *addrs, *tmp;
  getifaddrs(&addrs);
  tmp = addrs;
  while (tmp) {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET) { list.emplace_back(tmp->ifa_name); }
    tmp = tmp->ifa_next;
  }
  freeifaddrs(addrs);
  return list;
}

}  // namespace net
#include "Address.hpp"

class NetworkManager {
public:
private:
};

int main() { return 0; }
