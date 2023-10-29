//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <QtCore/QDataStream>
#include <QtExtensions/QtExtensions>

namespace Quasar::Datagrams
{
  struct StatusResponse
  {
    u32 marker;
    u16 voltage1;
    u16 voltage2;
    u8 switch_data;
    u8 sar_data;
    u8 r1;
    u8 r2;

    friend QDataStream& operator<<(QDataStream& dataStream, const StatusResponse& data);
    friend QDataStream& operator>>(QDataStream& dataStream, StatusResponse& data);
  };

  inline QDataStream& operator<<(QDataStream& s, const StatusResponse& d)
  {
    s << d.marker;
    s << d.voltage1;
    s << d.voltage2;
    s << d.switch_data;
    s << d.sar_data;
    s << d.r1;
    s << d.r2;
    return s;
  }

  inline QDataStream& operator>>(QDataStream& s, StatusResponse& d)
  {
    s >> d.marker;
    s >> d.voltage1;
    s >> d.voltage2;
    s >> d.switch_data;
    s >> d.sar_data;
    s >> d.r1;
    s >> d.r2;
    return s;
  }
} // Quasar::Datagrams
