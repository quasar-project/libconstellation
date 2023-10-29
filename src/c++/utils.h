//
// Created by whs31 on 29.10.2023.
//

#pragma once

#include <QtExtensions/QtExtensions>

namespace Quasar::Utils
{
  auto wrapToExecdString(const Qt::String& string, int* uid_counter = nullptr) noexcept -> Qt::ByteArray;
} // Quasar::Utils

