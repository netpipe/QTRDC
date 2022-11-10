QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS WEBP_HAVE_PNG WEBP_HAVE_JPEG
LIBS+= -lpng -ljpeg -lwebpmux -lwebpdemux -lwebp
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./ ./libwebp

SOURCES += \
    csslsocket.cpp \
    main.cpp \
    mainwindow.cpp \
    imageio/image_dec.c \
    imageio/image_enc.c \
    imageio/imageio_util.c \
    imageio/jpegdec.c \
    imageio/metadata.c \
    imageio/pngdec.c \
    imageio/pnmdec.c \
    imageio/tiffdec.c \
    imageio/webpdec.c \
    imageio/wicdec.c \
    libwebp/example_util.c \
    libwebp/utils/utils.c

HEADERS += \
    csslsocket.h \
    mainwindow.h \
    define.h \
    img2webp.h \
    imageio/image_dec.h \
    imageio/image_enc.h \
    imageio/imageio_util.h \
    imageio/jpegdec.h \
    imageio/metadata.h \
    imageio/pngdec.h \
    imageio/pnmdec.h \
    imageio/tiffdec.h \
    imageio/webpdec.h \
    imageio/wicdec.h \
    libwebp/example_util.h \
    libwebp/utils/utils.h \
    dwebp.h


FORMS += \
    mainwindow.ui

win32: { #mingw openssl precompiled binaries
    INCLUDEPATH += -L$$OUT_PWD\..\..\OpenSSL\include
    LIBS += -L"-L$$OUT_PWD\..\..\OpenSSL\lib\libssl.a"
    LIBS += -L"-L$$OUT_PWD\..\..\OpenSSL\lib\libcrypto.a"
}
unix: {
  #  LIBS += -L/usr/lib/x86_64-linux-gnu/mesa

    INCLUDEPATH += I-/usr/include/openssl
    LIBS +=  -ldl -lpthread -lcrypto
}
# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

#DEFINES += "PRO_PWD=\\\"$$_PRO_FILE_PWD_\\\""
