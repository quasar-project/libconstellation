#include <leaf/leaf.h>
#include <leaf/logger.h>
#include <constellation/ip/ipv4.h>
#include <constellation/network/modules/powerswitch.h>

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

  auto context = boost::asio::io_context();
  auto powerswitch = constellation::network::modules::PowerSwitch(
    "192.168.1.50",
    44000,
    context,
    std::chrono::seconds(5)
  );

  context.run();
  return 0;
}