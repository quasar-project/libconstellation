//
// Created by whs31 on 29.10.2023.
//

#include "utils.h"
#include <QtExtensionsNetworking/NetworkUtils>

namespace Quasar::Utils
{
  auto wrapToExecdString(const Qt::String& str, int* uid) noexcept -> Qt::ByteArray
  {
    using Qt::String;
    using namespace Qtx;

    const auto FILL = QLatin1Char('0');

    String ret = ":%1|%2|%3|";
    String crc = "%1";
    int uid_ctr = 1;
    if(uid != nullptr)
      uid_ctr = ++(*uid);
    ret = ret.arg(uid_ctr, 4, 16, FILL)
             .arg(str.length(), 2, 16, FILL)
             .arg(str);
    crc = crc.arg(NetworkUtils::checksum(NetworkUtils::stringToChar(ret), ret.length(), NetworkUtils::ChecksumMode::CRC16),
                  4, 16, FILL);
    ret += crc;
    return ret.toUtf8();
  }
} // Quasar::Utils