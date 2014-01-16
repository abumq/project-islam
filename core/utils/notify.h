#ifndef NOTIFY_H
#define NOTIFY_H

#include <cstdlib>
#include <string>
#include <sstream>
#include "core/logging/logging.h"

namespace {
static inline std::string& replaceAll(std::string& str, 
                                      const std::string& replaceWhat, 
                                      const std::string& replaceWith,
                                      int offset = 1) {
    if (replaceWhat == replaceWith)
        return str;
    std::size_t foundAt = std::string::npos;
    while ((foundAt = str.find(replaceWhat, foundAt + offset)) != std::string::npos) {
        str.replace(foundAt, replaceWhat.length(), replaceWith);
    }
    return str;
}

inline std::string toString(int val) 
{
#if defined(_MSC_VER)
    return std::to_string(val);
#else
    return __gnu_cxx::__to_xstring<std::string>(&std::vsnprintf,
                                                4 * sizeof(int),
                                                "%d", val);
#endif // defined(_MSC_VER)
}

int notify(const std::string& title, const std::string& message, int durationMs = 3000) {
    int result = 1;
#if defined(__linux__)
    std::stringstream ss;
    std::string m = message;
    std::string t = title;
    replaceAll(m, "\"", "\\\"", 2);
    replaceAll(t, "\"", "\\\"", 2);
    ss << "notify-send \"" << t << "\" \"" << m << "\" '-t' " 
       << toString(durationMs);
    LOG(INFO) << "Running [" << ss.str() << "]";
    result = system(ss.str().c_str());
#endif // defined(__linux__)
    return result;
}
} // namespace
#endif // NOTIFY_H
