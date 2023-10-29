//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <QtCore/QObject>
#include <QtExtensions/QtExtensions>
#include <QtExtensions/QMLRegistration>
#include <Quasar/Telemetry>
#include <Quasar/TelemetrySocket>

namespace Quasar
{
  class NetworkAPI : public Qt::Object
  {
    Q_OBJECT
    Q_PROPERTY(Quasar::Telemetry* telemetry READ telemetry CONSTANT FINAL)
    Q_PROPERTY(Quasar::TelemetrySocket* telemetrySocket READ telemetrySocket CONSTANT FINAL)

    public:
      static auto get() -> NetworkAPI*;
      static auto create(QQmlEngine*, QJSEngine*) -> NetworkAPI*;

      NetworkAPI(const NetworkAPI&) = delete;
      NetworkAPI(NetworkAPI&&) = delete;
      NetworkAPI& operator=(const NetworkAPI&) = delete;
      NetworkAPI& operator=(NetworkAPI&&) = delete;

      [[nodiscard]] Telemetry* telemetry() const;
      [[nodiscard]] TelemetrySocket* telemetrySocket() const;

    private:
      explicit NetworkAPI(Qt::Object* parent = nullptr);

    private:
      Telemetry* m_telemetry;
      TelemetrySocket* m_telemetry_socket;
  };
} // Quasar


