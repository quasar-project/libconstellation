#include <boost/asio.hpp>
#include <constellation/ip/ipv4.h>
#include <ip/netif.h>

using leaf::Err;
namespace asio = boost::asio;

namespace constellation::ip
{
  auto local_address_impl() -> expected<Ipv4, string>
  {
    auto addrs_v4 = gmlc::netif::getInterfaceAddressesV4();
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
    if(not local_address.has_value()) {
      llog::error("failed to resolve local ipv4 address, reason: {}", local_address.error());
      constellation::unrecoverable_error();
    }
    else
      llog::debug("resolved local ipv4 address: {}", local_address.value().to_string());
    return local_address;
  }

  auto Ipv4::local_address_unchecked() -> Ipv4 { return *Ipv4::local_address(); }
}
