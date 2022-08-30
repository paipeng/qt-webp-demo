QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cpwebp.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cpwebp.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    qt-webp-demo_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

#INCLUDEPATH += ../../C/libwebp-1.2.2/src





win32:CONFIG(release, debug|release): {
    INCLUDEPATH += C:/libwebp/include
    LIBS += -LC:/libwebp/lib -llibwebp
    }
else:win32:CONFIG(debug, debug|release): {
    INCLUDEPATH += C:/libwebp/include
    LIBS += -LC:/libwebp/lib -llibwebp
    }
else:unix: {
    INCLUDEPATH += /usr/local/Cellar/webp/1.1.0/include
    LIBS += -L/usr/local/lib -lwebp
}



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qt-webp-demo.qrc
