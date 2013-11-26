#ifndef LOGGING_H
#define LOGGING_H

#include <easylogging++.h>
#include "settings_loader.h"

class LoggingConfigurer {
public:
    static void configureLoggers() {
        
        el::Logger* defaultLogger = el::Loggers::getLogger("default");
        el::Logger* dataLogger = el::Loggers::getLogger("data");
        el::Logger* quranLogger = el::Loggers::getLogger("quran");
        el::Logger* traceLogger = el::Loggers::getLogger("locationTrace");
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
        
        traceLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Enabled, "true");
        traceLogger->reconfigure();
    }

    static el::Configurations baseConfiguration() {
    
        el::Configurations configurations;
        configurations.setToDefault();
        configurations.set(el::Level::Global, el::ConfigurationType::Filename, 
            SettingsLoader::defaultHomeDir().toStdString() + "logs" + QString(QDir::separator()).toStdString() + "project-islam.log");
        configurations.set(el::Level::Trace, el::ConfigurationType::Format, "%datetime %level %func");
        configurations.set(el::Level::Debug, el::ConfigurationType::Format, "%datetime %level [%func] %msg");
        // 2mb max log file size
        configurations.set(el::Level::Global, el::ConfigurationType::MaxLogFileSize, "2048000");
        return configurations;
    }
};

#define DATA_LOG(LEVEL) CLOG(LEVEL, "data")
#define DDATA_LOG(LEVEL) DCLOG(LEVEL, "data")
#define VDATA_LOG(vlevel) DCVLOG(vlevel, "data")

#define QURAN_LOG(LEVEL) CLOG(LEVEL, "quran")

#define _TRACE CLOG(TRACE, "locationTrace")

#endif // LOGGING_H
