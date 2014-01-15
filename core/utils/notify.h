#ifndef NOTIFY_H
#define NOTIFY_H

#include <cstdlib>
#include <string>
#include <sstream>
#include "core/logging/logging.h"

namespace {
inline std::string toString(int __val) 
{ 
    return __gnu_cxx::__to_xstring<std::string>(&std::vsnprintf,
                                           4 * sizeof(int),
                                           "%d", __val); 
}

int notify(const std::string& title, const std::string& message, int durationMs = 3000) {
    int result = 1;
#if defined(__linux__)
    std::stringstream ss;
    ss << "notify-send '" << title << "' '" << message << "' '-t' " 
       << toString(durationMs);
    LOG(INFO) << "Running [" << ss.str() << "]";
    result = system(ss.str().c_str());
#endif // defined(__linux__)
    return result;
}
} // namespace
#endif // NOTIFY_H
