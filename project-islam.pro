include(master.pri)

DEFINES += _ELPP_NO_DEFAULT_LOG_FILE

TARGET = project-islam
TEMPLATE = app



INCLUDEPATH += /usr/local/include/

## Utils headers
HEADERS += core/utils/memory.h \
    core/utils/version.h \
    core/utils/filesystem.h \
    core/utils/datetime.h \
    core/utils/notify.h

HEADERS  += mainwindow.h \
    core/constants.h \
    core/style_loader.h \
    core/settings_loader.h \
    core/logging/logging.h \
    core/logging/easylogging++.h \
    core/extension/extension_info.h \
    core/extension/extension_base.h \
    core/extension/abstract_extension.h \
    core/extension/extension_bar.h \
    core/extension/extension_item.h \
    core/extension/extension_loader.h \
    core/controls/colorbox.h \
    settings_dialog.h \
    core/quran/chapter.h \
    core/quran/quran.h \
    core/quran/verse.h \
    core/data/database_manager.h \
    core/data/database_manager.h \
    core/data/data_holder.h \
    core/download_manager.h \
    core/update/update_manager.h

SOURCES += main.cc\
    /usr/local/include/easylogging++.cc \
    mainwindow.cc \
    core/style_loader.cc \
    core/extension/abstract_extension.cc \
    core/extension/extension_loader.cc \
    core/extension/extension_bar.cc \
    core/extension/extension_item.cc \
    core/controls/colorbox.cc \
    core/settings_loader.cc \
    settings_dialog.cc \
    core/quran/verse.cc \
    core/quran/quran.cc \
    core/quran/chapter.cc \
    core/data/database_manager.cc \
    core/data/data_holder.cc \
    core/update/update_manager.cc \
    core/download_manager.cc

FORMS    += mainwindow.ui \
    settings_dialog.ui

OTHER_FILES += \
    resources/images/project-islam.png \
    resources/images/project-islam.ico \
    resources/configs/logging.linux.config \
    resources/configs/logging.win.config \
    extension.pri \
    master.pri \
    resources/styles/extension.qss \
    resources/styles/extension_bar.qss \
    resources/styles/extension_item.qss \
    resources/styles/menu.qss

RESOURCES += \
    resources/styles.qrc \
    resources/icons.qrc

ICON = resources/images/project-islam.ico
