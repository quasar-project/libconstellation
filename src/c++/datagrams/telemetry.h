//
// Created by whs31 on 29.10.2023.
//

#pragma once

#pragma once

#include <QtCore/QDataStream>
#include <QtExtensions/QtExtensions>

namespace Quasar::Datagrams
{
  struct TelemetryDatagram
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

    TelemetryDatagram& operator=(TelemetryDatagram d)
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

    friend QDataStream& operator<<(QDataStream& dataStream, const TelemetryDatagram& data);
    friend QDataStream& operator>>(QDataStream& dataStream, TelemetryDatagram& data);
  };

  struct TelemetryRequest
  {
    u32 marker = 0x55bb55bb;
    u8 init_flag = 0x00;
    u16 port = 0;
    u32 interval_ms = 0;
    u16 crc16 = 0x0;

    friend QDataStream& operator<<(QDataStream& dataStream, const TelemetryRequest& data);
    friend QDataStream& operator>>(QDataStream& dataStream, TelemetryRequest& data);
  };

  struct StatusDatagram
  {
    u32 marker;
    u16 voltage1;
    u16 voltage2;
    u8 switch_data;
    u8 sar_data;
    u8 r1;
    u8 r2;

    friend QDataStream& operator<<(QDataStream& dataStream, const StatusDatagram& data);
    friend QDataStream& operator>>(QDataStream& dataStream, StatusDatagram& data);
  };

  inline QDataStream& operator<<(QDataStream& dataStream, const TelemetryDatagram& data)
  {
    dataStream << data.marker;
    dataStream << data.version;
    dataStream << data.latitude;
    dataStream << data.longitude;
    dataStream << data.altitude;
    dataStream << data.velocity_course;
    dataStream << data.velocity_east;
    dataStream << data.velocity_north;
    dataStream << data.velocity_vertical;
    dataStream << data.pitch;
    dataStream << data.roll;
    dataStream << data.yaw;
    dataStream << data.course;
    dataStream << data.time;
    dataStream << data.satellites;
    dataStream << data.crc16;

    return dataStream;
  }

  inline QDataStream& operator>>(QDataStream& dataStream, TelemetryDatagram& data)
  {
    dataStream >> data.marker;
    dataStream >> data.version;
    dataStream >> data.latitude;
    dataStream >> data.longitude;
    dataStream >> data.altitude;
    dataStream >> data.velocity_course;
    dataStream >> data.velocity_east;
    dataStream >> data.velocity_north;
    dataStream >> data.velocity_vertical;
    dataStream >> data.pitch;
    dataStream >> data.roll;
    dataStream >> data.yaw;
    dataStream >> data.course;
    dataStream >> data.time;
    dataStream >> data.satellites;
    dataStream >> data.crc16;

    return dataStream;
  }

  inline QDataStream& operator<<(QDataStream& dataStream, const TelemetryRequest& data)
  {
    dataStream << data.marker;
    dataStream << data.init_flag;
    dataStream << data.port;
    dataStream << data.interval_ms;
    dataStream << data.crc16;

    return dataStream;
  }

  inline QDataStream& operator >>(QDataStream& dataStream, TelemetryRequest& data)
  {
    dataStream >> data.marker;
    dataStream >> data.init_flag;
    dataStream >> data.port;
    dataStream >> data.interval_ms;
    dataStream >> data.crc16;

    return dataStream;
  }

  inline QDataStream& operator<<(QDataStream& dataStream, const StatusDatagram& data)
  {
    dataStream << data.marker;
    dataStream << data.voltage1;
    dataStream << data.voltage2;
    dataStream << data.switch_data;
    dataStream << data.sar_data;
    dataStream << data.r1;
    dataStream << data.r2;

    return dataStream;
  }

  inline QDataStream& operator >>(QDataStream& dataStream, StatusDatagram& data)
  {
    dataStream >> data.marker;
    dataStream >> data.voltage1;
    dataStream >> data.voltage2;
    dataStream >> data.switch_data;
    dataStream >> data.sar_data;
    dataStream >> data.r1;
    dataStream >> data.r2;

    return dataStream;
  }
} // Quasar::Datagrams
