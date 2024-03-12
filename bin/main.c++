#include <leaf/leaf.h>
#include <leaf/logger.h>
#include <constellation/ip/ipv4.h>
#include <constellation/network/api.h>
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

  const auto api = constellation::network::NetworkAPI();
  QObject::connect(
    api.powerSwitch(),
    &constellation::network::modules::PowerSwitch::channelsChanged,
    [&]{ llog::warn("{}", api.powerSwitch()->channels().first().voltage()); }
  );

  return QCoreApplication::exec();
}