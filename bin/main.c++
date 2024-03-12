#include <leaf/leaf.h>
#include <leaf/logger.h>
#include <constellation/ip/ipv4.h>

auto main() -> int
{
  const auto logger = leaf::Logger(
    "test-logger",
    leaf::Logger::DefaultPatterns::SimpleWithThreadInfo,
    leaf::Logger::Level::Trace,
    leaf::Logger::Target::Stdout,
    std::nullopt,
    std::nullopt,
    std::nullopt
  );
  llog::info("bench started!");
  auto local_address = constellation::ip::Ipv4::local_address();
  if(local_address.has_value())
    llog::info("local ipv4 address: {}", local_address.value().to_string());
  else
    llog::error("failed to resolve local ipv4 address, reason: {}", local_address.error());
  return 0;
}