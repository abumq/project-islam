#ifndef LOGGING_H
#define LOGGING_H

#include "core/logging/easylogging++.h"
#include "core/settings_loader.h"
#include "core/utils/filesystem.h"

class LoggerConfig {
public:
    LoggerConfig(const std::string& id, bool debug = true, bool trace = true) :
        m_id(id), m_debugStr(debug ? "true" : "false"),
        m_traceStr(trace ? "true" : "false"){
        
    }
    const std::string& id() { return m_id; }
    const std::string& debug() { return m_debugStr; }
    const std::string& trace() { return m_traceStr; }
private:
    std::string m_id;
    std::string m_debugStr;
    std::string m_traceStr;
};

class LoggingConfigurer {
public:
    static void registerLoggers(std::vector<LoggerConfig>* cs) {
        for (LoggerConfig c : *cs) {
            el::Logger* logger = el::Loggers::getLogger(c.id());
            CHECK(logger != nullptr) << "Could not register logger [" << c.id() << "]";
            logger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, c.debug());
            logger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, c.trace());
            logger->reconfigure();
        }
    }
    
    static void configureLoggers() {
        
        LoggerConfig configsArr[] = {
            LoggerConfig("default"),
            LoggerConfig("data"),
            LoggerConfig("update_manager"),
            LoggerConfig("quran")
        };
        
        const int totalLoggerConfigs = sizeof(configsArr) / sizeof(configsArr[0]);
        std::vector<LoggerConfig> configs(configsArr, configsArr + totalLoggerConfigs);
        CHECK(!configs.empty()) << "We do not expect configs to be empty!";
        registerLoggers(&configs);
        
        el::Loggers::setDefaultConfigurations(baseConfiguration(), true);
        el::Helpers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    }
    
    static el::Configurations baseConfiguration() {
        
        el::Configurations configurations;
        configurations.setToDefault();
        std::string logFile = filesystem::buildFilename(QStringList() 
                                                        << SettingsLoader().defaultHomeDir() 
                                                        << "logs" << "project-islam.log").toStdString();
        configurations.set(el::Level::Global, el::ConfigurationType::Filename, logFile);
        configurations.set(el::Level::Trace, el::ConfigurationType::Format, "%datetime %level [%logger] %func %msg");
        configurations.set(el::Level::Debug, el::ConfigurationType::Format, "%datetime %level [%logger] [%func] %msg");
        // 2mb max log file size
        configurations.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "2048000");
        
        return configurations;
    }
};

inline MAKE_LOGGABLE(QVariant, qvariant, os) {
    os << qvariant.toString().toStdString().c_str();
    return os;
}

#endif // LOGGING_H
