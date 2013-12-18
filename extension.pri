include(master.pri)

## Target
TARGET = $$qtLibraryTarget($$NAME)
TEMPLATE = lib

DEFAULT_LOG_FILE = '\"\\".l.lg\\"\"'
LOGGER_ID = '\"\\"extension.$$NAME\\"\"'

DEFINES += _ELPP_DEFAULT_LOG_FILE=$$DEFAULT_LOG_FILE \
           _LOGGER=$$LOGGER_ID \
           _PERFORMANCE_LOGGER=$$LOGGER_ID

## Include base
INCLUDEPATH += "../../"
