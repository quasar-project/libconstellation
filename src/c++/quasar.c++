//
// Created by whs31 on 29.10.2023.
//

#include "quasar.h"

#include <QtExtensions/QMLRegistration>
#include <Quasar/NetworkAPI>

void Quasar::registerTypes(const char* uri, int major, int minor) noexcept
{
  qmlRegisterModule(uri, major, minor);
  qmlRegisterSingletonType<NetworkAPI>(uri, major, minor, "Quasar", NetworkAPI::create);
}