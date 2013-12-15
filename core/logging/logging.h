#ifndef LOGGING_H
#define LOGGING_H

#include "core/logging/easylogging++.h"
#include "core/settings_loader.h"
#include "core/utils/utils.h"

class LoggingConfigurer {
public:
    static void configureLoggers() {
        
        el::Logger* defaultLogger = el::Loggers::getLogger("default");
        el::Logger* dataLogger = el::Loggers::getLogger("data");
        el::Logger* updateManagerLogger = el::Loggers::getLogger("update_manager");
        el::Logger* quranLogger = el::Loggers::getLogger("quran");
        
        el::Loggers::setDefaultConfigurations(baseConfiguration(), true);
        
        defaultLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        defaultLogger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
        defaultLogger->reconfigure();
        
        dataLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        dataLogger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
        dataLogger->reconfigure();
        
        updateManagerLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        updateManagerLogger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
        updateManagerLogger->reconfigure();
        
        quranLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        quranLogger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
        quranLogger->reconfigure();
        
        el::Helpers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    }
    
    static el::Configurations baseConfiguration() {
        
        el::Configurations configurations;
        configurations.setToDefault();
        std::string logFile = utils::buildFilename(QStringList() 
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
