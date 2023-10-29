//
// Created by whs31 on 29.10.2023.
//

#include "telemetry.h"
#include <QtExtensions/Math>

namespace Quasar
{
  Telemetry::Telemetry(Qt::Object* parent)
    : Qt::Object(parent)
    , m_datagram({})
    , m_sea_level(.0f)
  {}

  Qt::GeoCoordinate Telemetry::position() const {
    return { m_datagram.latitude, m_datagram.longitude, m_datagram.altitude };
  }

  QVector2D Telemetry::velocity() const {
    return { static_cast<f32>(m_datagram.velocity_course), static_cast<f32>(m_datagram.velocity_vertical) };
  }

  QVector3D Telemetry::eulerAxes() const {
    return { static_cast<f32>(m_datagram.pitch), static_cast<f32>(m_datagram.yaw), static_cast<f32>(m_datagram.roll) };
  }

  int Telemetry::satellites() const { return m_datagram.satellites; }

  f32 Telemetry::seaLevel() const { return m_sea_level; }
  void Telemetry::setSeaLevel(f32 x) {
    if(Qtx::compare(x, m_sea_level))
      return;
    m_sea_level = x;
    emit seaLevelChanged();
  }

  void Telemetry::setDatagram(const Datagrams::TelemetryResponse& datagram)
  {
    m_datagram = datagram;

    emit positionChanged();
    emit velocityChanged();
    emit eulerAxesChanged();
    emit satellitesChanged();
  }
} // Quasar