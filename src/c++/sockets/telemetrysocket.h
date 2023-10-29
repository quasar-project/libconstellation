//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <QtExtensionsNetworking/UDPSocket>
#include <Quasar/Telemetry>

namespace Quasar
{
  class TelemetrySocket : public Qtx::UDPSocket
  {
    Q_OBJECT
    Q_PROPERTY(float interval READ interval WRITE setInterval NOTIFY intervalChanged FINAL)
    Q_PROPERTY(bool verifyChecksum READ verifyChecksum WRITE setVerifyChecksum NOTIFY verifyChecksumChanged FINAL)

    public:
      explicit TelemetrySocket(Telemetry* target, Qt::Object* parent = nullptr);

      invokable void start(const QString& request_address, const QString& response_address) noexcept;
      invokable void stop() noexcept;

      [[nodiscard]] f32 interval() const;         void setInterval(f32);
      [[nodiscard]] bool verifyChecksum() const;  void setVerifyChecksum(bool);

    signals:
      void intervalChanged();
      void verifyChecksumChanged();

      void ping();

    private:
      connectable_slot void process(QByteArray data) noexcept;
      connectable_slot void request() noexcept;

    private:
      Qt::Timer* m_update_timer;
      QUdpSocket* m_request_socket;
      Telemetry* m_target;
      f32 m_interval;
      bool m_verify_checksum;
      Qtx::NetworkUtils::IPV4Address m_request_addr;
  };
} // Quasar

Q_DECLARE_METATYPE(Quasar::TelemetrySocket*)