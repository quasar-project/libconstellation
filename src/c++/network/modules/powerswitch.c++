#include <span>
#include <vector>
#include <boost/asio.hpp>
#include <constellation/ip/ipv4.h>
#include <constellation/network/modules/powerswitch.h>

using std::span;
using std::vector;
using leaf::Err;
namespace asio = boost::asio;

namespace constellation::network::modules
{
  constexpr u16 LOCAL_PORT = 12000;
  constexpr u16 DUMMY_CHANNEL = 9'999;
  constexpr auto REQUEST_MARKER = 0xAAAAAAAA;

  PowerSwitch::PowerSwitch(
    const string_view ipv4,
    const u16 port,
    asio::io_context& context,
    const std::chrono::seconds request_interval
  )
    : m_socket(context, asio::ip::udp::endpoint()),
      m_endpoint(this->m_socket.local_endpoint()),
      m_timer(context),
      m_request_interval(request_interval),
      m_buffer(array<u8, 1024>()),
      m_channels(array<ChannelData, 8>())
  {
    this->configure(ipv4, port, request_interval)
      .map_error([](const auto& e){ llog::error("failed to initialize powerswitch: {}", e); });
  }

  PowerSwitch::~PowerSwitch() { this->stop(); }

  auto PowerSwitch::toggle_channel(const int channel) -> void
  {
    const auto packet = RequestPacket {
      .marker = REQUEST_MARKER,
      .channel = static_cast<u16>(channel),
      .response_port = this->m_endpoint.port(),
      .checksum = 0x0000
    };

    this->write({ reinterpret_cast<const char*>(&packet), sizeof(packet) });
  }

  auto PowerSwitch::stop() -> void
  {
    this->m_socket.close();
    llog::trace("closing connection to {}", this->m_endpoint.address().to_string());
  }

  auto PowerSwitch::configure(
    const string_view ipv4,
    const u16 port,
    const std::chrono::seconds request_interval
  ) -> expected<void, string>
  {
    this->stop();
    try {
      this->m_endpoint = asio::ip::udp::endpoint(
        asio::ip::make_address_v4(ip::Ipv4::local_address_unchecked().address),
        LOCAL_PORT
      );

      llog::trace("opening socket at {}:{}", this->m_endpoint.address().to_string(), this->m_endpoint.port());

      this->m_socket.open(this->m_endpoint.protocol());
      this->m_socket.bind(this->m_endpoint);
      this->m_request_interval = request_interval;

      this->m_target = asio::ip::udp::endpoint(
        asio::ip::make_address_v4(ipv4),
        port
      );

      this->m_timer = asio::steady_timer(
        this->m_socket.get_executor(),
        this->m_request_interval
      );
      this->handle_timer();

      llog::debug("powerswitch service started at {}:{} (receiving from {}:{})",
        this->m_socket.local_endpoint().address().to_string(),
        this->m_socket.local_endpoint().port(),
        this->m_target.address().to_string(),
        this->m_target.port()
      );
    } catch(const std::exception& e) {
      return Err("exception: {}", e.what());
    }
    this->read();
    return {};
  }

  auto PowerSwitch::request() -> void {
    llog::trace("powerswitch: sending planned request");
    this->toggle_channel(DUMMY_CHANNEL);
  }

  auto PowerSwitch::read() -> void
  {
    this->m_socket.async_receive_from(
      asio::buffer(this->m_buffer),
      this->m_endpoint,
      [this](const auto& ec, const auto& bytes_transferred)
      {
        if(ec) {
          llog::error("powerswitch: error: {}", ec.message());
          return;
        }
        const auto raw = span(this->m_buffer).first(bytes_transferred);
        for(const auto datagram = reinterpret_cast<array<ResponsePacket, 8>*>(raw.data());
          const auto& [marker, channel, enabled, voltage, current] : *datagram) {
          this->m_channels[channel] = ChannelData {
            .voltage = static_cast<f32>(voltage) / 1'000.0f,
            .current = static_cast<f32>(current),
            .enabled = static_cast<bool>(enabled)
          };
        }
        llog::trace("[:{} {}V {} mA]", this->m_channels.front().enabled, this->m_channels.front().voltage, this->m_channels.front().current);

        this->read();
      }
    );
  }

  auto PowerSwitch::handle_timer() -> void
  {
    this->request();
    this->m_timer.expires_from_now(this->m_request_interval);
    this->m_timer.async_wait(
      [this](const auto& ec)
      {
        if(ec) {
          llog::error("powerswitch: error: {}", ec.message());
          return;
        }
        this->handle_timer();
      }
    );
  }

  auto PowerSwitch::write(const string_view data) -> void
  {
    this->m_socket.async_send_to(
      asio::buffer(data),
      this->m_target,
      [this](const auto& ec, const auto& bytes_transferred)
      {
        if(ec) {
          llog::error("powerswitch: error: {}", ec.message());
          return;
        }
        llog::trace("powerswitch: sent {} bytes to {}:{}",
          bytes_transferred,
          this->m_target.address().to_string(),
          this->m_target.port()
        );

        this->read();
      }
    );
  }
}
