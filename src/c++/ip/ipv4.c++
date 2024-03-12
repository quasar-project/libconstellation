#include <boost/asio.hpp>
#include <constellation/ip/ipv4.h>

#if !defined(_WIN32)
#include <ip/netif.h>
#else
#include <qnetworkinterface.h>
#endif

using std::vector;
using leaf::Err;
namespace asio = boost::asio;

namespace constellation::ip
{
  auto local_address_impl() -> expected<Ipv4, string>
  {
    #if defined(_WIN32)
    const auto interfaces = QNetworkInterface::allAddresses();
    auto addrs_v4_all = vector<string>();
    for(const auto& interface : interfaces)
      if(not interface.isLoopback() and interface.protocol() == QAbstractSocket::IPv4Protocol)
        addrs_v4_all.emplace_back(interface.toString().toStdString());
    if(addrs_v4_all.empty())
      return Err("no addresses found for local interface between {} interfaces", interfaces.size());
    #endif
    auto addrs_v4 =
      #if defined(_WIN32)
      addrs_v4_all;
      #else
      gmlc::netif::getInterfaceAddressesV4();
      #endif
    std::erase_if(addrs_v4, [](const std::string& addr) { return addr == "127.0.0.1"; });
    if(addrs_v4.empty())
      return Err("failed to resolve ipv4 address of host machine: no ipv4 addresses found!");

    auto result = Ipv4 {
      .address = asio::ip::make_address_v4(addrs_v4.front()).to_uint()
    };
    return result;
  }

  auto Ipv4::to_string() const -> string {
    return fmt::format("{}.{}.{}.{}", address >> 24, (address >> 16) & 0xFF, (address >> 8) & 0xFF, address & 0xFF);
  }

  auto Ipv4::local_address() -> expected<Ipv4, string>
  {
    static auto local_address = local_address_impl();
    return local_address;
  }

  auto Ipv4::local_address_unchecked() -> Ipv4
  {
    const auto addr = Ipv4::local_address();
    if(not addr.has_value()) {
      llog::error("failed to resolve local ipv4 address, reason: {}", addr.error());
      constellation::unrecoverable_error();
    }
    else
      llog::debug("resolved local ipv4 address: {}", addr.value().to_string());
    return *addr;
  }
}
