#pragma once

#include <constellation/global.h>

namespace constellation::ip
{
  struct Ipv4
  {
    u32 address;

    [[nodiscard]] auto to_string() const -> string;

    [[nodiscard]] static auto local_address() -> expected<Ipv4, string>;
    [[nodiscard]] static auto local_address_unchecked() -> Ipv4;
  };
}