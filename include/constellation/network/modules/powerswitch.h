#pragma once

#include <array>
#include <memory>
#include <chrono>
#include <leaf/pattern/iobservable.h>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <constellation/global.h>

namespace constellation::network::modules
{
  using std::unique_ptr;
  using std::array;

  class PowerSwitch : public leaf::pattern::IObservable<int>
  {
    struct [[gnu::packed]] RequestPacket
    {
      u32 marker;
      u16 channel;
      u16 response_port;
      u16 checksum;
    };

    struct [[gnu::packed]] ResponsePacket
    {
      u16 marker;
      u8 channel;
      u8 enabled;
      u32 voltage;        ///< Напряжение в милливольтах
      u32 current;        ///< Ток в миллиамперах
    };

    struct ChannelData
    {
      f32 voltage;        ///< Напряжение в вольтах
      f32 current;        ///< Ток в миллиамперах
      bool enabled;       ///< Состояние канала (вкл/выкл)
    };

    public:
      explicit PowerSwitch(string_view ipv4, u16 port, boost::asio::io_context& context, std::chrono::seconds request_interval);
      ~PowerSwitch();

      [[nodiscard]] auto channel_status(int channel) -> ChannelData;
      auto toggle_channel(int channel) -> void;

      auto stop() -> void;

    private:
      auto configure(string_view ipv4, u16 port, std::chrono::seconds request_interval) -> expected<void, string>;
      auto request() -> void;
      auto read() -> void;
      auto handle_incoming(usize bytes) -> void;
      auto handle_timer() -> void;
      auto write(string_view data) -> void;

    private:
      boost::asio::ip::udp::socket m_socket;
      boost::asio::ip::udp::endpoint m_endpoint;
      boost::asio::ip::udp::endpoint m_target;
      boost::asio::deadline_timer m_timer;
      std::chrono::seconds m_request_interval;
      array<u8, 1024> m_buffer;
      array<ChannelData, 8> m_channels;
  };
}