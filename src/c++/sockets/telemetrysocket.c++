//
// Created by whs31 on 29.10.2023.
//

#include "telemetrysocket.h"
#include <QtCore/QTimer>
#include <QtCore/QDataStream>
#include <QtExtensions/Logging>
#include <QtExtensions/Math>

using namespace Qtx;

namespace Quasar
{
  TelemetrySocket::TelemetrySocket(Quasar::Telemetry* target, Qt::Object* parent)
    : UDPSocket(parent)
    , m_update_timer(new Qt::Timer(this))
    , m_request_socket(new QUdpSocket(this))
    , m_target(target)
    , m_interval(.2f)
    , m_verify_checksum(true)
    , m_request_addr(Qtx::NetworkUtils::IPV4Address())
  {
    this->setName("Telemetry");

    Qt::Object::connect(m_update_timer, &Qt::Timer::timeout, this, &TelemetrySocket::request);
    Qt::Object::connect(this, &TelemetrySocket::received, this, &TelemetrySocket::process);
  }

  void TelemetrySocket::start(const QString& request_address, const QString& response_address) noexcept
  {
    auto parse = Qtx::NetworkUtils::parseIPv4String(request_address);
    if(not parse.has_value())
    {
      llog(Error) "Incorrect address provided:" << request_address;
      return;
    }

    llog(Info) "Starting UDP socket named" << name() << "on" << request_address;
    m_request_addr = parse.value();
    this->UDPSocket::start(response_address);
    this->request();
    m_update_timer->start(30 * 1'000);
    llog(Debug) "Started reading with interval" << interval();
  }

  void TelemetrySocket::stop() noexcept
  {
    this->UDPSocket::stop();
    m_update_timer->stop();
  }

  f32 TelemetrySocket::interval() const { return m_interval; }
  void TelemetrySocket::setInterval(f32 x) { m_interval = x; emit intervalChanged(); }
  bool TelemetrySocket::verifyChecksum() const { return m_verify_checksum; }
  void TelemetrySocket::setVerifyChecksum(bool x) { m_verify_checksum = x; emit verifyChecksumChanged(); }

  void TelemetrySocket::process(QByteArray data) noexcept
  {
    using Qt::ByteArray;
    using Qt::DataStream;
    using Qt::String;

    const float MS_TO_KMH = 3.6f;

    auto marker = *(u32*)data.data();
    if(marker != Datagrams::TelemetryMarker::TelemetryResponseMarker)
    {
      llog(Warning) "Received unknown marker:" << Qt::hex << marker << Qt::dec;
      return;
    }

    DataStream stream(&data, ReadOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    Datagrams::TelemetryResponse response;
    stream >> response;
    response.latitude = rad2deg(response.latitude);
    response.longitude = rad2deg(response.longitude);
    response.velocity_course = response.velocity_course * MS_TO_KMH;
    response.velocity_east = response.velocity_east * MS_TO_KMH;
    response.velocity_north = response.velocity_north * MS_TO_KMH;
    response.velocity_vertical = response.velocity_vertical * MS_TO_KMH;
    response.pitch = rad2deg(response.pitch);
    response.roll = rad2deg(response.roll);
    response.yaw = rad2deg(response.yaw);
    response.course = response.yaw;
    response.satellites = response.satellites >> 1;

    m_target->setDatagram(response);
    u16 crc = verifyChecksum() ? NetworkUtils::checksum((const char*)&response,
                                                        sizeof(Datagrams::TelemetryResponse) - sizeof(u16),
                                                        NetworkUtils::ChecksumMode::CRC16CCIITT)
                               : response.crc16;
    if(crc != response.crc16)
      llog(Warning) "Checksum mismatch [" << crc << ":" << response.crc16 << "]";
    emit metrics(response.toString(), sizeof(response), false);
    emit ping();
  }

  void TelemetrySocket::request() noexcept
  {
    using Qt::ByteArray;
    using Qt::DataStream;
    using Qt::String;

    ByteArray buffer;
    DataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
    Datagrams::TelemetryRequest request = {
        .marker = Datagrams::TelemetryMarker::TelemetryRequestMarker,
        .init_flag = 0x01,
        .port = port16(),
        .interval_ms = static_cast<u32>(interval() * 1'000),
        .crc16 = 0
    };
    request.crc16 = NetworkUtils::checksum((const char*)&request,
                                           sizeof(Datagrams::TelemetryRequest) - sizeof(u16),
                                           QtEx::NetworkUtils::ChecksumMode::CRC16CCIITT);
    stream << request;
    m_request_socket->writeDatagram(buffer, m_request_addr.address, m_request_addr.port);
    emit metrics(request.toString(), sizeof(request), true);
  }
} // Quasar