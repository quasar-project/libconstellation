//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <QtCore/QDataStream>
#include <QtExtensions/QtExtensions>

namespace Quasar::Datagrams
{
  enum TelemetryMarker : u32
  {
    StatusRequestMarker = 0x55FF55FF,
    StatusResponseMarker = 0xEE55EE55,
    TelemetryRequestMarker = 0x55BB55BB,
    TelemetryResponseMarker = 0xAA55AA55
  };

  struct TelemetryResponse
  {
    u32 marker = 0x0;
    u8 version = 0x0;
    f64 latitude = 0;
    f64 longitude = 0;
    f64 altitude = 0;
    f64 velocity_course = 0;
    f64 velocity_east = 0;
    f64 velocity_north = 0;
    f64 velocity_vertical = 0;
    f64 pitch = 0;
    f64 roll = 0;
    f64 yaw = 0;
    f64 course = 0;
    u64s time = 0;
    u8 satellites = 0;
    u16 crc16 = 0x0;

    [[nodiscard]] Qt::String toString() const noexcept;

    TelemetryResponse& operator=(TelemetryResponse d)
    {
      marker = d.marker;
      version = d.version;
      latitude = d.latitude;
      longitude = d.longitude;
      altitude = d.altitude;
      velocity_course = d.velocity_course;
      velocity_east = d.velocity_east;
      velocity_north = d.velocity_north;
      velocity_vertical = d.velocity_vertical;
      pitch = d.pitch;
      roll = d.roll;
      yaw = d.yaw;
      course = d.course;
      time = d.time;
      satellites = d.satellites;
      crc16 = d.crc16;
      return *this;
    }

    friend QDataStream& operator<<(QDataStream& dataStream, const TelemetryResponse& data);
    friend QDataStream& operator>>(QDataStream& dataStream, TelemetryResponse& data);
  };

  struct TelemetryRequest
  {
    u32 marker = TelemetryMarker::TelemetryRequestMarker;
    u8 init_flag = 0x00;
    u16 port = 0;
    u32 interval_ms = 0;
    u16 crc16 = 0x0;

    [[nodiscard]] Qt::String toString() const noexcept;

    friend QDataStream& operator<<(QDataStream& dataStream, const TelemetryRequest& data);
    friend QDataStream& operator>>(QDataStream& dataStream, TelemetryRequest& data);
  };
} // Quasar::Datagrams

namespace Quasar::Datagrams
{
  inline Qt::String TelemetryResponse::toString() const noexcept
  {
    return Qt::String("0x%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 0x%16")
        .arg(marker, 0, 16)
        .arg(version)
        .arg(latitude, 0, 'g', 7)
        .arg(longitude, 0, 'f', 7)
        .arg(altitude, 0, 'f', 2)
        .arg(velocity_course, 0, 'f', 1)
        .arg(velocity_east, 0, 'f', 1)
        .arg(velocity_north, 0, 'f', 1)
        .arg(velocity_vertical, 0, 'f', 1)
        .arg(pitch, 0, 'f', 2)
        .arg(roll, 0, 'f', 2)
        .arg(yaw, 0, 'f', 2)
        .arg(course, 0, 'f', 2)
        .arg(time)
        .arg(satellites)
        .arg(crc16, 0, 16);
  }

  inline QDataStream& operator<<(QDataStream& s, const TelemetryResponse& d)
  {
    s << d.marker;
    s << d.version;
    s << d.latitude;
    s << d.longitude;
    s << d.altitude;
    s << d.velocity_course;
    s << d.velocity_east;
    s << d.velocity_north;
    s << d.velocity_vertical;
    s << d.pitch;
    s << d.roll;
    s << d.yaw;
    s << d.course;
    s << d.time;
    s << d.satellites;
    s << d.crc16;
    return s;
  }

  inline QDataStream& operator>>(QDataStream& s, TelemetryResponse& d)
  {
    s >> d.marker;
    s >> d.version;
    s >> d.latitude;
    s >> d.longitude;
    s >> d.altitude;
    s >> d.velocity_course;
    s >> d.velocity_east;
    s >> d.velocity_north;
    s >> d.velocity_vertical;
    s >> d.pitch;
    s >> d.roll;
    s >> d.yaw;
    s >> d.course;
    s >> d.time;
    s >> d.satellites;
    s >> d.crc16;
    return s;
  }

  inline Qt::String TelemetryRequest::toString() const noexcept
  {
    return Qt::String("0x%1 %2 %3 %4 0x%5")
        .arg(marker, 0, 16)
        .arg(init_flag)
        .arg(port)
        .arg(interval_ms)
        .arg(crc16, 0, 16);
  }

  inline QDataStream& operator<<(QDataStream& s, const TelemetryRequest& d)
  {
    s << d.marker;
    s << d.init_flag;
    s << d.port;
    s << d.interval_ms;
    s << d.crc16;
    return s;
  }

  inline QDataStream& operator>>(QDataStream& s, TelemetryRequest& d)
  {
    s >> d.marker;
    s >> d.init_flag;
    s >> d.port;
    s >> d.interval_ms;
    s >> d.crc16;
    return s;
  }
} // Quasar::Datagrams
