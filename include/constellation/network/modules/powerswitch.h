#pragma once

#include <array>
#include <memory>
#include <chrono>
#include <qobject.h>
#include <constellation/global.h>

class QTimer;
class QUdpSocket;

namespace constellation::network::modules
{
  using std::thread;
  using std::unique_ptr;
  using std::array;

  class ChannelData
  {
    Q_GADGET
    Q_PROPERTY(float voltage READ voltage WRITE setVoltage FINAL)
    Q_PROPERTY(float current READ current WRITE setCurrent FINAL)
    Q_PROPERTY(bool status READ status WRITE setStatus FINAL)

    public:
      ChannelData();
      ChannelData(f32 voltage, f32 current, bool status);

      [[nodiscard]] float voltage() const;
      void setVoltage(float value);

      [[nodiscard]] float current() const;
      void setCurrent(float value);

      [[nodiscard]] bool status() const;
      void setStatus(bool value);

    private:
      f32 m_voltage;      ///< Напряжение в вольтах
      f32 m_current;      ///< Ток в миллиамперах
      bool m_status;      ///< Состояние канала (вкл/выкл)
  };

  // ReSharper disable once CppClassCanBeFinal
  class PowerSwitch : public QObject
  {
    Q_OBJECT
    Q_PROPERTY(QList<ChannelData> channels READ channels NOTIFY channelsChanged FINAL)

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
      u32 voltage; ///< Напряжение в милливольтах
      u32 current; ///< Ток в миллиамперах
    };

    public:
      explicit PowerSwitch(string_view ipv4, u16 port, std::chrono::seconds request_interval);
      virtual ~PowerSwitch() override;

      [[nodiscard]] QList<ChannelData> channels() const;

      Q_INVOKABLE void toggleChannel(int channel) const;
      Q_INVOKABLE void stop() const;
      Q_INVOKABLE bool start(const QString& ip, const u16 port, f32 request_interval_seconds);

    signals:
      void channelsChanged();

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

Q_DECLARE_METATYPE(constellation::network::modules::ChannelData)
Q_DECLARE_METATYPE(constellation::network::modules::PowerSwitch*)