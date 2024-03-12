#include <span>
#include <vector>
#include <qtimer.h>
#include <qudpsocket.h>
#include <qhostaddress.h>
#include <constellation/ip/ipv4.h>
#include <constellation/network/modules/powerswitch.h>

using std::span;
using std::vector;
using std::make_unique;
using leaf::Err;

namespace constellation::network::modules
{
  constexpr u16 LOCAL_PORT = 12000;
  constexpr u16 DUMMY_CHANNEL = 9'999;
  constexpr auto REQUEST_MARKER = 0xAAAAAAAA;

  PowerSwitch::PowerSwitch(
    const string_view ipv4,
    const u16 port,
    const std::chrono::seconds request_interval
  )
    : m_target({ipv4, port})
    , m_socket(make_unique<QUdpSocket>(nullptr))
    , m_scheduler(make_unique<QTimer>(nullptr))
    , m_request_interval(request_interval)
    , m_buffer(array<u8, 1024>())
    , m_channels(array<ChannelData, 8>())
  {
    this->configure(ipv4, port, request_interval)
        .map_error([](const auto& e) { llog::error("failed to initialize powerswitch: {}", e); });
  }

  PowerSwitch::~PowerSwitch() { this->stop(); }

  auto PowerSwitch::toggle_channel(const int channel) const -> void
  {
    const auto packet = RequestPacket{
      .marker = REQUEST_MARKER,
      .channel = static_cast<u16>(channel),
      .response_port = LOCAL_PORT,
      .checksum = 0x0000
    };

    this->write({reinterpret_cast<const char*>(&packet), sizeof(packet)});
  }

  auto PowerSwitch::stop() const -> void
  {
    llog::trace("closing connection to {}", this->m_socket->localAddress().toString().toStdString());
    this->m_socket->close();
    this->m_scheduler->stop();
  }

  auto PowerSwitch::configure(
    const string_view ipv4,
    const u16 port,
    const std::chrono::seconds request_interval
  ) -> expected<void, string>
  {
    this->stop();

    llog::trace("opening socket at {}:{}", ip::Ipv4::local_address_unchecked().to_string(), LOCAL_PORT);
    this->m_socket->bind(QHostAddress(ip::Ipv4::local_address_unchecked().address), LOCAL_PORT);
    this->m_request_interval = request_interval;
    this->m_target = {ipv4, port};

    this->request();
    this->m_scheduler = make_unique<QTimer>(nullptr);
    this->m_scheduler->start(request_interval);
    QObject::connect(this->m_scheduler.get(), &QTimer::timeout, [this] {
      this->request();
    });
    QObject::connect(this->m_socket.get(), &QUdpSocket::readyRead, [this]{
      this->read();
    });

    llog::debug("powerswitch service started at {}:{} (receiving from {}:{})",
                ip::Ipv4::local_address_unchecked().to_string(),
                LOCAL_PORT,
                this->m_target.ip,
                this->m_target.port
    );
    return {};
  }

  auto PowerSwitch::request() const -> void
  {
    llog::trace("powerswitch: sending planned request");
    this->toggle_channel(DUMMY_CHANNEL);
  }

  auto PowerSwitch::read() -> void
  {
    while(this->m_socket->hasPendingDatagrams())
    {
      QByteArray data;
      data.resize(static_cast<int>(this->m_socket->pendingDatagramSize()));
      this->m_socket->readDatagram(data.data(), data.size());
      for(const auto datagram = reinterpret_cast<array<ResponsePacket, 8>*>(data.data());
          const auto& [marker, channel, enabled, voltage, current] : *datagram)
      {
        this->m_channels[channel] = ChannelData{
          .voltage = static_cast<f32>(voltage) / 1'000.0f,
          .current = static_cast<f32>(current),
          .enabled = static_cast<bool>(enabled)
        };
      }
    }
  }

  auto PowerSwitch::write(const string_view data) const -> void
  {
    this->m_socket->writeDatagram(
      data.data(),
      static_cast<u64>(data.size()),
      QHostAddress(QString::fromStdString(string(this->m_target.ip))),
      this->m_target.port
    );
  }
}
