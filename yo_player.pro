QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = $$PWD/ico/yo_player.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    music_class/playlist.cpp \
    music_class/track.cpp \
    musicslider.cpp

HEADERS += \
    mainwindow.h \
    music_class/playlist.h \
    music_class/track.h \
    musicslider.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/BASS/c/ -lbass
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/BASS/c/ -lbass

INCLUDEPATH += $$PWD/BASS/c
DEPENDPATH += $$PWD/BASS/c

win32: LIBS += -L$$PWD/BASS_flac/c/ -lbassflac

INCLUDEPATH += $$PWD/BASS_flac/c
DEPENDPATH += $$PWD/BASS_flac/c

RESOURCES += \
    resurse.qrc







win32: LIBS += -L$$PWD/tags/c/ -ltags

INCLUDEPATH += $$PWD/tags/c
DEPENDPATH += $$PWD/tags/c

DISTFILES += \
    playlist


