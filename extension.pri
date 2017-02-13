include(master.pri)

## Target
TARGET = $$qtLibraryTarget($$NAME)
TEMPLATE = lib

DEFAULT_LOG_FILE = '\"\\".l.lg\\"\"'
LOGGER_ID = '\"\\"extension.$$NAME\\"\"'
PERFORMANCE_LOGGER_ID = '\"\\"extension.performance.$$NAME\\"\"'

DEFINES += ELPP_DEFAULT_LOG_FILE=$$DEFAULT_LOG_FILE \
           LOGGER=$$LOGGER_ID \
           PERFORMANCE_LOGGER=$$PERFORMANCE_LOGGER_ID \
           ELPP_AS_DLL \
           ELPP_EXPORT_SYMBOLS \
           PROJECT_ISLAM_EXTENSION


INCLUDEPATH += /usr/local/include/

SOURCES +=  \
    ../../core/settings_loader.cc \
    ../../core/extension/abstract_extension.cc \
    ../../core/extension/extension_item.cc \
    ../../core/extension/extension_bar.cc \
    /usr/local/include/easylogging++.cc \

HEADERS += \
    ../../core/settings_loader.h \
    ../../core/extension/abstract_extension.h \
    ../../core/extension/extension_base.h \
    ../../core/extension/extension_bar.h \
    ../../core/extension/extension_item.h \
    ../../core/extension/extension_info.h

## Include base
INCLUDEPATH += "../../"
