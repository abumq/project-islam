#ifndef LOGGING_H
#define LOGGING_H

#define _ELPP_DISABLE_DEBUG_LOGS
#define _ELPP_DISABLE_TRACE_LOGS

#include "core/logging/easylogging++.h"
#include "core/settings_loader.h"
#include "core/utils/filesystem.h"

// We redefine TIMED_FUNC and TIMED_SCOPE to use different level of logging
static const el::Level kPerformanceLoggingLevel = el::Level::Debug;

class LoggerConfig {
public:
    LoggerConfig(const std::string& id, bool debug = true, bool trace = true,
        bool toFile = true) :
        m_id(id), m_debugStr(debug ? "true" : "false"),
        m_traceStr(trace ? "true" : "false") ,
        m_toFile(toFile ? "true" : "false") {
        
    }
    const std::string& id() { return m_id; }
    const std::string& debug() { return m_debugStr; }
    const std::string& trace() { return m_traceStr; }
    const std::string& toFile() { return m_toFile; }
private:
    std::string m_id;
    std::string m_debugStr;
    std::string m_traceStr;
    std::string m_toFile;
};

class LoggingConfigurer {
public:
    static void registerLoggers(std::vector<LoggerConfig>* cs) {
        for (LoggerConfig c : *cs) {
            el::Logger* logger = el::Loggers::getLogger(c.id());
            CHECK(logger != nullptr) << "Could not register logger [" << c.id() << "]";
            logger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, c.debug());
            logger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, c.trace());
            logger->configurations()->set(el::Level::Global, el::ConfigurationType::ToFile, c.toFile());
            logger->reconfigure();
        }
    }
    
    static void reconfigurePerformanceLogger(el::Logger* logger) {
        if (logger == nullptr) {
            return;
        }
        logger->configurations()->set(kPerformanceLoggingLevel, el::ConfigurationType::Format, "%datetime %level [%logger] %msg");
        logger->reconfigure();
    }
    
    static void configureLoggers() {
        LoggerConfig configsArr[] = {
            LoggerConfig("default"),
            LoggerConfig("data"),
            LoggerConfig("update_manager"),
            LoggerConfig("settings_loader", true, true, false),
            LoggerConfig("quran")
        };
        const int totalLoggerConfigs = sizeof(configsArr) / sizeof(configsArr[0]);
        std::vector<LoggerConfig> configs(configsArr, configsArr + totalLoggerConfigs);
        CHECK(!configs.empty()) << "We do not expect configs to be empty!";
        registerLoggers(&configs);
        el::Loggers::setDefaultConfigurations(baseConfiguration(), true);
        el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    }
    
    static el::Configurations baseConfiguration() {
        
        el::Configurations configurations;
        configurations.setToDefault();
        std::string logFile = filesystem::buildFilename(QStringList() 
                                                        << SettingsLoader::getInstance()->defaultHomeDir() 
                                                        << "logs" << "project-islam.log").toStdString();
        configurations.set(el::Level::Global, el::ConfigurationType::Filename, logFile);
        configurations.set(el::Level::Trace, el::ConfigurationType::Format, "%datetime %level [%logger] %func %msg");
        configurations.set(el::Level::Debug, el::ConfigurationType::Format, "%datetime %level [%logger] [%func] %msg");
        configurations.set(el::Level::Error, el::ConfigurationType::Format, "%datetime %level [%logger] [%loc] %msg");
        // 2mb max log file size
        configurations.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "2048000");
        
        return configurations;
    }
};

inline MAKE_LOGGABLE(QVariant, qvariant, os) {
    os << qvariant.toString().toStdString().c_str();
    return os;
}

#define _TRACE std::stringstream ss

#endif // LOGGING_H
