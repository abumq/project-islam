include(master.pri)
include(api.pri)

TARGET = project-islam
TEMPLATE = app
HEADERS  += mainwindow.h \
    settings_dialog.h
    
SOURCES += main.cc \
        mainwindow.cc \
    style_loader.cc
    
FORMS    += controls/quran_reader.ui \
    controls/quran_reciter.ui \
    mainwindow.ui \
    settings_dialog.ui
    
OTHER_FILES += \
    api.pri

RESOURCES += \
    resources/styles.qrc \
    resources/icons.qrc
    
    
INCLUDEPATH += include/

ICON = resources/images/project-islam.ico
