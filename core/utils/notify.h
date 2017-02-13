#ifndef NOTIFY_H
#define NOTIFY_H

#include <cstdlib>
#include <string>
#include <sstream>
#include "core/logging/logging.h"
#include <QSystemTrayIcon>

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
    return std::to_string(val);
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
    DLOG(INFO) << "Running [" << ss.str() << "]";
    result = system(ss.str().c_str());
#endif // defined(__linux__)
    return result;
}

void notifyGeneral(QSystemTrayIcon* trayIcon, const QString& title, const QString& message, int durationMs, QSystemTrayIcon::MessageIcon icon) {
    if (trayIcon == nullptr) {
        notify(title.toStdString(), message.toStdString(), durationMs);
        return;
    }
    trayIcon->showMessage(title, message, icon, durationMs);
}

void notifyInfo(QSystemTrayIcon* trayIcon, const QString& title, const QString& message, int durationMs = 3000) {
    notifyGeneral(trayIcon, title, message, durationMs, QSystemTrayIcon::Information);
}

void notifyWarn(QSystemTrayIcon* trayIcon, const QString& title, const QString& message, int durationMs = 3000) {
    notifyGeneral(trayIcon, title, message, durationMs, QSystemTrayIcon::Warning);
}

void notifyError(QSystemTrayIcon* trayIcon, const QString& title, const QString& message, int durationMs = 3000) {
    notifyGeneral(trayIcon, title, message, durationMs, QSystemTrayIcon::Critical);
}
} // namespace
#endif // NOTIFY_H
