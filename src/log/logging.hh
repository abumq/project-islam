#ifndef LOGGING_HH
#define LOGGING_HH

#include "log/easylogging++.h"

static void configureLoggers(void) {
    el::Loggers::configureFromGlobal("../config/logging.conf");
    el::Loggers::setDefaultConfigurations(*el::Loggers::getLogger("default")->configurations(), false);
}

#define DATABASE_LOG(LEVEL) CLOG(LEVEL, "database")
#define QURAN_LOG(LEVEL) CLOG(LEVEL, "quran")

#endif // LOGGING_HH
