#-------------------------------------------------
#
# Project created by QtCreator 2019-12-17T16:36:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QVideoStyler
TEMPLATE = app

DEFINES += WEBP_HAVE_PNG WEBP_HAVE_JPEG
LIBS+= -lpng -ljpeg -lwebpmux -lwebpdemux -lwebp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
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
        mainwindow.h \
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
    libwebp/utils/utils.h

FORMS += \
        mainwindow.ui
