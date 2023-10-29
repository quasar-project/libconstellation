//
// Created by whs31 on 29.10.2023.
//

#include "networkapi.h"
#include <QtCore/QCoreApplication>

namespace Quasar
{
  auto NetworkAPI::get() -> NetworkAPI* { static NetworkAPI instance; return &instance; }
  auto NetworkAPI::create(QQmlEngine*, QJSEngine*) -> NetworkAPI* { return get(); }
  NetworkAPI::NetworkAPI(Qt::Object* parent)
    : Qt::Object(parent)
    , m_config(std::make_unique<Qtx::StaticConfig>(
        QCoreApplication::applicationDirPath() + "/config/quasar-config.json",
        ":/quasar/quasar-config.json"
    ))
    , m_telemetry(new Telemetry(this))
    , m_telemetry_socket(new TelemetrySocket(telemetry(), this))
  {}

  Qtx::StaticConfig* NetworkAPI::config() const { return m_config.get(); }
  Telemetry* NetworkAPI::telemetry() const { return m_telemetry; }
  TelemetrySocket* NetworkAPI::telemetrySocket() const { return m_telemetry_socket; }
} // Quasar