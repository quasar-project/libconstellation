#include <leaf/leaf.h>
#include <leaf/logger.h>
#include <constellation/ip/ipv4.h>
#include <constellation/network/modules/powerswitch.h>
#include <qcoreapplication.h>

auto main(int argc, char* argv[]) -> int
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

  QCoreApplication app(argc, argv);

  auto powerswitch = constellation::network::modules::PowerSwitch(
    "192.168.1.50",
    44000,
    std::chrono::seconds(5)
  );

  return QCoreApplication::exec();
}