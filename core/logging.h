#ifndef LOGGING_H
#define LOGGING_H

#include "core/easylogging++.h"
#include "core/settings_loader.h"
#include <QVariant>

class LoggingConfigurer {
public:
    static void configureLoggers() {
        
        el::Logger* defaultLogger = el::Loggers::getLogger("default");
        el::Logger* dataLogger = el::Loggers::getLogger("data");
        el::Loggers::getLogger("quran");
        /*
         // Having issue with path
         // We need configureFromGlobalText(confText)
         QString logConfigFilename;
        #if _ELPP_OS_UNIX
            logConfigFilename = QApplication::applicationDirPath() + QDir::separator() + "configs/logging.linux.config";
        #elif _ELPP_OS_WINDOWS
            logConfigFilename = ":/logging_config/win";
        #endif
        
        el::Loggers::configureFromGlobal(logConfigFilename.toStdString().c_str());
        */
        
        el::Loggers::setDefaultConfigurations(baseConfiguration(), true);
        
        dataLogger->configurations()->set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
        dataLogger->reconfigure();
        
        defaultLogger->configurations()->set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
        defaultLogger->reconfigure();
    }
    
    static el::Configurations baseConfiguration() {
        
        el::Configurations configurations;
        configurations.setToDefault();
        std::string logFile = (QStringList() << SettingsLoader().defaultHomeDir() << "logs")
            .join(QDir::separator()).append(QDir::separator()).toStdString() + "project-islam.log";

        configurations.set(el::Level::Global, el::ConfigurationType::Filename, 
            logFile);
        configurations.set(el::Level::Trace, el::ConfigurationType::Format, "%datetime %level %func %msg");
        configurations.set(el::Level::Debug, el::ConfigurationType::Format, "%datetime %level [%func] %msg");
        // 2mb max log file size
        configurations.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "2048000");
        return configurations;
    }
};

inline MAKE_LOGGABLE(QVariant, qvariant, os) {
    os << qvariant.toString().toStdString().c_str();
    return os;
}

#define DATA_LOG(LEVEL) CLOG(LEVEL, "data")
#define DDATA_LOG(LEVEL) DCLOG(LEVEL, "data")
#define VDATA_LOG(vlevel) DCVLOG(vlevel, "data")

#define QURAN_LOG(LEVEL) CLOG(LEVEL, "quran")

#define _TRACE CLOG(TRACE, "default")

#endif // LOGGING_H
