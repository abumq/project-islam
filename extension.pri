include(master.pri)

## Target
TARGET = $$qtLibraryTarget($$NAME)
TEMPLATE = lib

DEFAULT_LOG_FILE = '\"\\".l.lg\\"\"'
LOGGER_ID = '\"\\"extension.$$NAME\\"\"'
PERFORMANCE_LOGGER_ID = '\"\\"extension.performance.$$NAME\\"\"'

DEFINES += _ELPP_DEFAULT_LOG_FILE=$$DEFAULT_LOG_FILE \
           _LOGGER=$$LOGGER_ID \
           _PERFORMANCE_LOGGER=$$PERFORMANCE_LOGGER_ID

## Include base
INCLUDEPATH += "../../"
