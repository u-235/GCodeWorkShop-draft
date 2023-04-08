
VERSION = 2018.07
TEMPLATE = lib
TARGET = edytornc-common
CONFIG += staticlib

QT *= widgets serialport network

include(../common.pri)

INCLUDEPATH += include

#EdytorNC common

SOURCES += \
    serialtransmission.cpp \
    serialtransmissiondialog.cpp \
    serialportconfigdialog.cpp

HEADERS += \
    commoninc.h \
    serialportsettings.h \
    serialtransmission.h \
    serialtransmissiondialog.h \
    serialportconfigdialog.h \
    serialportsettings.h

FORMS += \
    transmissiondialog.ui \
    serialtransmissiondialog.ui \
    serialportconfigdialog.ui


# utils
#######################################

HEADERS += include/ui/longjobhelper.h \
    include/utils/medium.h \
    include/utils/configpage.h \
    include/utils/configdialog.h

SOURCES += ui/longjobhelper.cpp \
    utils/medium.cpp \
    utils/configpage.cpp \
    utils/configdialog.cpp

FORMS += utils/configdialog.ui


# resources
#######################################

RESOURCES = application.qrc
