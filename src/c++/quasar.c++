//
// Created by whs31 on 29.10.2023.
//

#include "quasar.h"

#include <QtExtensions/QMLRegistration>
#include <Quasar/NetworkAPI>

inline void initResources()
{
  #if (QT_VERSION < QT_VERSION_CHECK(6, 2, 0))
  Q_INIT_RESOURCE(quasar_rc);
  #endif
}

void Quasar::registerTypes(const char* uri, int major, int minor) noexcept
{
  initResources();

  qmlRegisterModule(uri, major, minor);
  qmlRegisterSingletonType<NetworkAPI>(uri, major, minor, "Quasar", NetworkAPI::create);
}