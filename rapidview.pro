QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG  += c++17 no_keywords

QMAKE_CXXFLAGS += -fvisibility=hidden

#QMAKE_CXXFLAGS_DEBUG -=
#QMAKE_CXXFLAGS_DEBUG +=

QMAKE_CXXFLAGS_RELEASE -= -O2 -O1
QMAKE_CXXFLAGS_RELEASE += -O3

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    src/jsontab.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/opentabs.cpp \
    src/settings.cpp \

HEADERS += \
    src/jsontab.h \
    src/mainwindow.h \
    src/opentabs.h \
    src/settings.h

FORMS += \
    ui/jsontab.ui \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
