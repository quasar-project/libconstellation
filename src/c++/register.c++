//
// Created by whs31 on 29.10.2023.
//

#include "register.h"
#include <QtExtensions/QMLRegistration>
#include <Quasar/NetworkAPI>

namespace Quasar
{
  void registerTypes(const char* uri, int major, int minor) noexcept
  {
    qmlRegisterModule(uri, major, minor);
    qmlRegisterSingletonType<NetworkAPI>(uri, major, minor, "QuasarNetworkAPI", NetworkAPI::create);
  }
} // Quasar