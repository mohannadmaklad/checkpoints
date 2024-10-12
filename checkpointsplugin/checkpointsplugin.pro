include( ../plugin.pri )

# Put intermediate files in the build directory
MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
RCC_DIR     = build/rcc
UI_DIR      = build/ui

TARGET = $$qtLibraryTarget(dltcheckpointsplugin)

CONFIG += c++17

*-g++* {
    QMAKE_CXXFLAGS += -std=c++17
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Wextra
    QMAKE_CXXFLAGS += -DPLUGIN_INSTALLATION_PATH=\\\"$$PREFIX/usr/share/dlt-viewer/plugins\\\"
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addSequenceDialog.cpp \
    checkpointsPlugin.cpp \
    form.cpp \
    sequences.cpp

HEADERS += \
    addSequenceDialog.h \
    checkpointsPlugin.h \
    form.h \
    sequences.h

FORMS += \
    addChpDialog.ui \
    form.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    CMakeLists.txt
