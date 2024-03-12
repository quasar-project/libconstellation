#pragma once

#include <string>
#include <sstream>
#include <ranges>
#include <leaf/leaf.h>
#include <boost/stacktrace/stacktrace.hpp>

namespace constellation
{
  using std::string;
  using std::string_view;
  using namespace leaf::types;

  [[noreturn]] auto unrecoverable_error() -> void;
}

namespace constellation
{
  inline auto unrecoverable_error() -> void
  {
    llog::error("unrecoverable error occured in constellation. see stacktrace below:");
    const auto stacktrace = boost::stacktrace::stacktrace();
    const auto stacktrace_string = (std::ostringstream() << stacktrace).str();
    auto splitted = std::vector<string>();
    auto j = 0;
    for(auto i = 0; i < stacktrace_string.size(); ++i) {
      if(stacktrace_string[i] == '\n') {
        splitted.emplace_back(stacktrace_string.substr(j, i - j));
        j = i + 1;
      }
      if(i == stacktrace_string.size() - 1)
        splitted.emplace_back(stacktrace_string.substr(j, i - j + 1));
      if(splitted.size() > 10)
        break;
    }
    for(const auto& line : splitted)
      llog::trace(">> {}", string_view(line));
    std::terminate();
  }
}
