DEFINES += _ELPP_NO_DEFAULT_LOG_FILE

## Utils headers
HEADERS += include/utils/memory.h \
    include/utils/version.h \
    include/utils/filesystem.h \
    include/utils/datetime.h

HEADERS  += \
    include/constants.h \
    include/style_loader.h \
    include/settings_loader.h \
    include/logging/logging.h \
    include/logging/easylogging++.h \
    include/extension/extension_info.h \
    include/extension/extension_base.h \
    include/extension/abstract_extension.h \
    include/extension/extension_bar.h \
    include/extension/extension_item.h \
    include/extension/extension_loader.h \
    include/controls/colorbox.h \
    include/controls/quran_view.h \
    include/controls/quran_reader.h \
    include/controls/quran_reciter.h \
    include/quran/chapter.h \
    include/quran/quran.h \
    include/quran/verse.h \
    include/data/database_manager.h \
    include/data/database_manager.h \
    include/data/data_holder.h \
    include/download_manager.h \
    include/update/update_manager.h

SOURCES += \
    extension/abstract_extension.cc \
    extension/extension_loader.cc \
    extension/extension_bar.cc \
    extension/extension_item.cc \
    controls/colorbox.cc \
    controls/quran_view.cc \
    controls/quran_reader.cc \
    controls/quran_reciter.cc \
    settings_loader.cc \
    quran/verse.cc \
    quran/quran.cc \
    quran/chapter.cc \
    data/database_manager.cc \
    data/data_holder.cc \
    update/update_manager.cc \
    download_manager.cc

FORMS    += controls/quran_reader.ui \
    controls/quran_reciter.ui

OTHER_FILES += \
    resources/styles/menu.style \
    resources/styles/extension_bar.style \
    resources/styles/extension.style \
    resources/styles/extension_item.style \
    resources/images/project-islam.png \
    resources/images/project-islam.ico \
    resources/configs/logging.linux.config \
    resources/configs/logging.win.config \
    extension.pri \
    master.pri
    
INCLUDEPATH += include/
