//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <memory>
#include <QtCore/QObject>
#include <QtExtensions/QtExtensions>
#include <QtExtensions/QMLRegistration>
#include <QtExtensions/StaticConfig>
#include <Quasar/Telemetry>
#include <Quasar/TelemetrySocket>

using std::unique_ptr;

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

      [[nodiscard]] Qtx::StaticConfig* config() const;
      [[nodiscard]] Telemetry* telemetry() const;
      [[nodiscard]] TelemetrySocket* telemetrySocket() const;

    private:
      explicit NetworkAPI(Qt::Object* parent = nullptr);

    private:
      unique_ptr<Qtx::StaticConfig> m_config;
      Telemetry* m_telemetry;
      TelemetrySocket* m_telemetry_socket;
  };
} // Quasar


