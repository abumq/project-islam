## Modules
QT       += core gui widgets sql multimedia network


unix:!win32 {
    ## Compiler
    COMPILER = icpc
    
    ## Standard
    QMAKE_CXXFLAGS += -std=c++0x

    ## Warning suppressions
    QMAKE_CXXFLAGS += -Wno-sign-compare
    
    ## Other flags
    QMAKE_CXXFLAGS += -fPIC -g
    
    QMAKE_CC = $$COMPILER
    QMAKE_CXX = $$COMPILER
    QMAKE_LINK = $$COMPILER

    ## Unix specific defines
    DEFINES += _ELPP_STACKTRACE_ON_CRASH _ELPP_HANDLE_POST_PERFORMANCE_TRACKING
}

DEFINES += _ELPP_UNICODE \
           _ELPP_QT_LOGGING \
           _ELPP_STL_LOGGING \
           _ELPP_LOG_UNORDERED_MAP \
           _ELPP_THREAD_SAFE

win32 {
    message("Using temporary fixes for Windows build")

    ## Deals with QDateTime compilation issue
    ## See http://qt-project.org/forums/viewthread/22133
    DEFINES += NOMINMAX

    ## Temporary fix - need proper solution!
    INCLUDEPATH += "C:\\Users\\Majid\\Dropbox\\project-islam\\"
}
