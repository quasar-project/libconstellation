//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <QtCore/QObject>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>
#include <QtPositioning/QGeoCoordinate>
#include <QtExtensions/QtExtensions>
#include <Quasar/Data/Telemetry>

namespace Quasar
{
  class Telemetry : public Qt::Object
  {
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate position READ position NOTIFY positionChanged FINAL)
    Q_PROPERTY(QVector2D velocity READ velocity NOTIFY velocityChanged FINAL)
    Q_PROPERTY(QVector3D eulerAxes READ eulerAxes NOTIFY eulerAxesChanged FINAL)
    Q_PROPERTY(int satellites READ satellites NOTIFY satellitesChanged FINAL)
    Q_PROPERTY(float seaLevel READ seaLevel WRITE setSeaLevel NOTIFY seaLevelChanged FINAL)

    public:
      explicit Telemetry(Qt::Object* parent = nullptr);

      [[nodiscard]] Qt::GeoCoordinate position() const;
      [[nodiscard]] QVector2D velocity() const;
      [[nodiscard]] QVector3D eulerAxes() const;
      [[nodiscard]] int satellites() const;
      [[nodiscard]] f32 seaLevel() const;

      void setSeaLevel(f32);
      void setDatagram(const Datagrams::TelemetryResponse& datagram);

    signals:
      void positionChanged();
      void velocityChanged();
      void eulerAxesChanged();
      void satellitesChanged();
      void seaLevelChanged();

    private:
      Datagrams::TelemetryResponse m_datagram;
      f32 m_sea_level;
  };
} // Quasar

Q_DECLARE_METATYPE(Quasar::Telemetry*)

