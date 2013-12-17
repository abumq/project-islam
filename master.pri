## Compiler
COMPILER = clang++
## Modules
QT       += core gui widgets sql multimedia network

## Standard
QMAKE_CXXFLAGS += -std=c++0x

## Warning suppressions
QMAKE_CXXFLAGS += -Wno-sign-compare

## Other flags
QMAKE_CXXFLAGS += -fPIC -g

QMAKE_CC = $$COMPILER
QMAKE_CXX = $$COMPILER
QMAKE_LINK = $$COMPILER

DEFINES += _ELPP_UNICODE \
           _ELPP_QT_LOGGING \
           _ELPP_STL_LOGGING \
           _ELPP_LOG_UNORDERED_MAP \
           _ELPP_STACKTRACE_ON_CRASH \
           _ELPP_THREAD_SAFE

# libimf for Intel C++
# TODO: export LD_LIBRARY_PATH
LIBS += -L/home/mkhan/intel/composer_xe_2013.5.192/compiler/lib/intel64/
