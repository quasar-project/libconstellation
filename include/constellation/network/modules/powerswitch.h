#pragma once

#include <array>
#include <memory>
#include <chrono>
#include <leaf/pattern/iobservable.h>
#include <constellation/global.h>

class QTimer;
class QUdpSocket;

namespace constellation::network::modules
{
  using std::thread;
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
      explicit PowerSwitch(string_view ipv4, u16 port, std::chrono::seconds request_interval);
      ~PowerSwitch();

      [[nodiscard]] auto channel_status(int channel) -> ChannelData;
      auto toggle_channel(int channel) const -> void;
      auto stop() const -> void;

    private:
      auto configure(string_view ipv4, u16 port, std::chrono::seconds request_interval) -> expected<void, string>;
      auto request() const -> void;
      auto read() -> void;
      auto write(string_view data) const -> void;

    private:
      struct endpoint
      {
        string_view ip;
        u16 port;
      } m_target;
      unique_ptr<QUdpSocket> m_socket;
      unique_ptr<QTimer> m_scheduler;
      std::chrono::seconds m_request_interval;
      array<u8, 1024> m_buffer;
      array<ChannelData, 8> m_channels;
  };
}